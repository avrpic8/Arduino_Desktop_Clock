#include "src/myImports.h"


/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
void onEspDisconnected(const WiFiEventStationModeDisconnected& event);
void blinkLED();
void intiPins(void);
void initTwi(void);
void initLcd(void);
void tryConnectToWifi(void);
void initWifiModule(void);
bool initNtpClient(void);
void initRtc(void);
void updateRTC(void);
void enableRotaryMenuInterrupt(void);
void disableRotaryMenuInterrupt(void);
void showMainMenu(void);
void showClockPage(void);
void showClockSetting(void);
void showClockSettingTitles(void);
void timeSet(void);
void dateSet(void);

void setup()
{
    Serial.begin(115200);
    
    intiPins();
    initTwi();
    initLcd();
    initRtc();
    
    /// start ticker
    ledTicker.attach(1, blinkLED);

    WiFi.mode(WIFI_OFF);
    ui.clearScreen();
}

IRAM_ATTR void checkPosition()
{
  menuIdx = menu.getMenuIndex();
  menu.setInputTime(millis());
}

void loop()
{ 
  showClockPage();
  showMainMenu();
}


////////// Implementaion methods  //////////

void blinkLED(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  ui.checkDisplaySleep();
}

void intiPins(void){
  pinMode(LED_BUILTIN, OUTPUT);
}

void initTwi(void){
  wire.begin(SDA,SCL);
  wire.setClock(1000000);
}

void initLcd(void){
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire, -1);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
  }
  display.clearDisplay();
  
  ui = Ui(&display);
  ui.setDisplaySleepTime(30);
  ui.setContrast(1);
}

void tryConnectToWifi(void){
  ui.printStringAt(0,0 , "Connecting...");
  if(!wifiManger.autoConnect("Wifi-Clock")){
    ui.clearScreen(); 
    ui.printStringAt(0,0 , "Unable to connected");
    delay(2000);
    return;
  }
  ui.clearScreen(); 
  ui.printStringAt(0,0 , "Connected to " + wifiManger.getWiFiSSID());
  delay(2000);    
}

void initWifiModule(void){
    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);
    disconnectedEvent = WiFi.onStationModeDisconnected(&onEspDisconnected);
    tryConnectToWifi();      
}

bool initNtpClient(void){
  ui.clearScreen();
  ui.printStringAt(0, 0, "Updating NTP");
  timeClient.begin();
  timeClient.setTimeOffset(12600);
  timeClient.setUpdateInterval(0);

  if(!timeClient.update()){
    ui.clearScreen();
    ui.printStringAt(0, 0, "NTP update failed");
    delay(2000);
    return false;
  }else{
    ui.clearScreen();
    ui.printStringAt(0, 0, "NTP update success");   
    delay(2000);
    return true; 
  }
  
}

void initRtc(void){
  ui.clearScreen();
  while (!rtc.begin())
  {
    Serial.println("RTC not found");
    delay(1000);
  }

  ui.printStringAt(0, 0, "Setting RTC");
  if(!rtc.setEpoch(timeClient.getEpochTime())){
    ui.clearScreen();
    ui.printStringAt(0, 0, "RTC not found");
  }else{
    ui.clearScreen();
    ui.printStringAt(0, 0, "RTC set success");  
  }
  delay(2000);
}

void updateRTC(void){
  if(rtcUpdateCounter == 20){
    ui.clearScreen();
    tryConnectToWifi();
    if(initNtpClient()){
      ui.clearScreen();
      rtc.setEpoch(timeClient.getEpochTime());  
      WiFi.mode(WIFI_OFF);
      ui.printStringAt(0, 0, "Wifi turned off"); 
      delay(2000);
    }else{
      ui.clearScreen();   
      ui.printStringAt(0, 0, "RTC update failed");
      ui.clearScreen();  
      WiFi.mode(WIFI_OFF);
      ui.printStringAt(0, 0, "Wifi turned off"); 
      delay(2000);
    }
    rtcUpdateCounter = 0;
    ui.clearScreen();
  }
}

void onEspConnected(const WiFiEventStationModeConnected& event){
    Serial.print("connected to the wifi: ");
    Serial.println(event.ssid);
    conectedFlag = true;     
}

void onEspDisconnected(const WiFiEventStationModeDisconnected& event){
    Serial.print("disconnected from the wifi: ");
    Serial.println(event.ssid);
    conectedFlag = false;     
}

void enableRotaryMenuInterrupt(void){
  attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_IN2), checkPosition, CHANGE);
}

void disableRotaryMenuInterrupt(void){
  detachInterrupt(PIN_ROTARY_IN1);
  detachInterrupt(PIN_ROTARY_IN2);;
}

char getBatteryLevel(void){
  int adc = analogRead(ANALOG_PIN);
  int result = (adc * 2) / 100;
  if(result > 20) result = 20;
  return result;
}

void showMainMenu(void){

  ui.disableDisplaySleep();
  menuIdx = 0;
  menu.setInputTime(millis());
  menu.setMaxMargin(5);
  enableRotaryMenuInterrupt();

  ui.clearScreen();
  ui.enableDefaultFont();

  while (true)
  { 
    ui.printAppBar(35,3, "Dashboard");
    if(menu.checkForAutoExit()) {
      disableRotaryMenuInterrupt();
      ui.enableDisplaySleep();
      ui.clearScreen();
      break;
    }
    switch (menuIdx) {
      case Home:
            ui.printStringAt(1, 16, "o");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "clock", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "wifi settings", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "system", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "display", false);

            ui.updateScreen();

          if(menu.checkMenuSwitch() == CLICKED){
            disableRotaryMenuInterrupt();
            ui.enableDisplaySleep();
            ui.clearScreen();
            return;
          }
        break;

      case Clock_SETTING:
 
            ui.clearDisplayAt(1, 16, " ");
            ui.printStringAt(1, 26, "o");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "clock", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "wifi settings", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "system", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "display", false);

            ui.updateScreen();
            if(menu.checkMenuSwitch() == CLICKED) showClockSetting();
        break;

      case WIFI_SETTING:
 
            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.printStringAt(1, 36, "o");
            ui.clearDisplayAt(1, 46, " ");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "clock", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "wifi settings", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "system", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "display", false);

            ui.updateScreen();
      
        break;

      case SYSTEM:

            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.printStringAt(1, 46, "o");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "clock", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "wifi settings", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "system", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "display", false);

            ui.updateScreen();

      break;

      case DISPLAY:

            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.printStringAt(1, 56, "o");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "clock", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "wifi settings", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "system", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "display", false);

            ui.updateScreen();
 
        break;

      case EXIT:
 
            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.printStringAt(1, 56, "o");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "clock", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "wifi settings", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "system", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "display", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "exit", false);

            ui.updateScreen();

        if(menu.checkMenuSwitch() == CLICKED){
            disableRotaryMenuInterrupt();
            ui.clearScreen();
            ui.enableDisplaySleep();
            return;
        }    
        break;
    }
  }
}

void showClockPage(){

  display.setTextColor(WHITE, BLACK);
  while(1){

    ///updateRTC();
    /// get time from ds1307 rtc
    uint16 year;
    uint8_t hour, min, sec, temp, week;
    rtc.getDateTime(&hour, &min, &sec, &temp, &temp, &year, &week);

    char clickStatus = menu.checkMenuSwitch();
    if(clickStatus == CLICKED){
      ui.displayOn();
    }
    if(clickStatus == LONG_CLICKED){
      return;
    }
    if(ui.isDisplayTimeOut()){
      ui.displayOff();
    }

    ui.displayWeek(0, 0, 2, week);
    ui.displayHour(0,22,4, hour);
    ui.displayColon(44, 30,3);
    ui.displayMin(56,22,4, min);
    ui.displaySec(102, 35, 2, sec);
    ui.displayDate(0, 55, 1, ui.epochToDate(rtc.getEpoch()));
    ui.showBatteryPercentage(getBatteryLevel());
    ui.showTemprature(110, 55, 1, 25);
    ui.updateScreen();  
  }
}

void showClockSetting(void){
  ui.clearScreen();
  menu.setMaxMargin(3);
  menuIdx = 0;

  while(true){
    switch (menuIdx){
    case 0:
      showClockSettingTitles();
      ui.printStringAt(1, 16, "o");
      ui.printStringAt(1, 26, " ");
      ui.clearDisplayAt(1, 36, " ");
      ui.clearDisplayAt(1, 46, " ");
      ui.updateScreen();
      if(menu.checkMenuSwitch() == CLICKED) timeSet();
      break;
    
    case 1:
      showClockSettingTitles();
      ui.printStringAt(1, 16, " ");
      ui.clearDisplayAt(1, 26, "o");
      ui.clearDisplayAt(1, 36, " ");
      ui.clearDisplayAt(1, 46, " ");
      ui.updateScreen();
      if(menu.checkMenuSwitch() == CLICKED) dateSet();
      break;  

    case 2:
      showClockSettingTitles();
      ui.printStringAt(1, 16, " ");
      ui.clearDisplayAt(1, 26, " ");
      ui.clearDisplayAt(1, 36, "o");
      ui.clearDisplayAt(1, 46, " ");
      ui.updateScreen();
      break;  
    
    case 3:
      showClockSettingTitles();
      ui.printStringAt(1, 16, " ");
      ui.clearDisplayAt(1, 26, " ");
      ui.clearDisplayAt(1, 36, " ");
      ui.clearDisplayAt(1, 46, "o");
      ui.updateScreen();
      if(menu.checkMenuSwitch() == CLICKED){
        menu.setMaxMargin(5);
        menuIdx = 1;  
        ui.clearScreen();
        return;
      }
      break;  
    }
  }
}

void showClockSettingTitles(void){
  ui.printAppBar(35,3, "Clock Menu");
  ui.printStringAt(10, 16, "time", false);
  ui.printStringAt(10, 26, "date", false);
  ui.printStringAt(10, 36, "alarm", false);
  ui.printStringAt(10, 46, "exit", false);
}

void timeSet(void){
  ui.clearScreen();
  menu.setMaxMargin(2);
  menuIdx = 0;

  /// get time from ds1307 rtc
  uint8_t hour, min, sec;
  rtc.getTime(&hour, &min, &sec);
  ui.displayHour(0,30,3, hour);
  ui.displayColon(35, 30,3);
  ui.displayMin(56,30,3, min);
  ui.displaySec(102, 35, 2, sec);
  while(true){
    switch (menuIdx)
    {
    case 0:
      display.fillTriangle(10, 3, 30, 3, 20, 15, WHITE);
      menu.setMaxMargin(23);
      while (true)
      {
        hour = menuIdx;
        ui.displayHour(0,30,3, hour); 
        if(menu.checkMenuSwitch() == CLICKED){
          menuIdx = 1;
          break;
        }  
        ui.updateScreen(); 
      }
      break;
    
    case 1:
      display.fillRect(10, 3, 25, 15, BLACK);
      display.fillRect(107, 3, 25, 15, BLACK);
      display.fillTriangle(60, 3, 80, 3, 70, 15, WHITE);
      menu.setMaxMargin(59);
      while (true)
      {
        min = menuIdx;
        ui.displayMin(56,30,3, min); 
        if(menu.checkMenuSwitch() == CLICKED){
          menuIdx = 2;
          break;
        }   
        ui.updateScreen();
      }
      break;

    case 2:
      display.fillRect(10, 3, 25, 15, BLACK);
      display.fillRect(60, 3, 25, 15, BLACK);
      display.fillTriangle(107, 3, 127, 3, 117, 15, WHITE);
      menu.setMaxMargin(59);
      while (true)
      {
        sec = menuIdx;
        ui.displaySec(102, 35, 2, sec); 
        if(menu.checkMenuSwitch() == CLICKED){
          menu.setMaxMargin(3);
          menuIdx = 0;
          ui.clearScreen();
          ui.enableDefaultFont();
          while(!rtc.setTime(hour, min, sec));
          return;
        }   
        ui.updateScreen();
      }
      break;
    }
  }
}

void dateSet(void){
  ui.clearScreen();
  menu.setMaxMargin(3);
  menuIdx = 0;  

  ui.printNumberAt(0, 25, 2, 12);
  ui.printStringAt(32, 25, 2, "Jun");
  ui.printStringAt(80, 25, 2, "2022");
  ui.printStringAt(32, 47, 2, "Wed");

  u_int8_t day, month, week;
  u_char year;

  while (true)
  {
    switch (menuIdx)
    {
    case 0:
      display.fillTriangle(5, 3, 25, 3, 15, 15, WHITE);
      menu.setMaxMargin(31);
      menuIdx = 1;
      while (true)
      {
        day = menuIdx;
        if(menu.checkMenuSwitch() == CLICKED){
          menuIdx = 1;
          break;
        } 
        ui.printNumberAt(0, 25, 2, menuIdx);
        ui.updateScreen();
      }
      break;

    case 1:
      display.fillTriangle(42, 3, 62, 3, 52, 15, WHITE);
      display.fillRect(5, 3 , 25, 15, BLACK);
      menu.setMaxMargin(11);
      menuIdx = 1;
      while (true)
      {
        month = menuIdx + 1;
        if(menu.checkMenuSwitch() == CLICKED){
          menuIdx = 2;
          break;
        }  
        ui.displayMonth(32, 25, 2, menuIdx);
        ui.updateScreen();
      }
      break;  

    case 2:
      display.fillTriangle(90, 3, 110, 3, 100, 15, WHITE);
      display.fillRect(42, 3 , 25, 15, BLACK);
      menu.setMaxMargin(30);
      menuIdx = 22;
      while (true)
      {
        year = 2000 + menuIdx;
        if(menu.checkMenuSwitch() == CLICKED){
          menuIdx = 3;
          break;
        }  
        ui.printNumberAt(80, 25, 2, 2000 + menuIdx, 4);
        ui.updateScreen();   
      }
      break;

    case 3:
      display.fillTriangle(10, 43, 10, 60, 20, 51, WHITE);
      display.fillRect(90, 3 , 25, 15, BLACK);
      menu.setMaxMargin(6);
      menuIdx = 0;
      while (true)
      {
        week = menuIdx;
        if(menu.checkMenuSwitch() == CLICKED){
          menu.setMaxMargin(3);
          menuIdx = 0;
          ui.clearScreen();
          ui.enableDefaultFont();
          while (!rtc.setDate(day, month, year-4, week));
          return;
        } 
        ui.displayWeek(32, 47, 2, menuIdx);
        ui.updateScreen();  
      }
      break; 
    }
  }  
}