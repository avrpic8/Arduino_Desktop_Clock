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
    // initWifiModule();
    // initNtpClient();
    initRtc();
    
    /// start ticker
    ledTicker.attach(1, blinkLED);
    alarmClock = Alarm(&alarmOn, &alarmOff, D3);
    sensor.begin();

    turnWifiOff();
    ui.clearScreen();  
}

IRAM_ATTR void checkPosition()
{
  menuIdx = menu.getMenuIndex();
  menu.setInputTime(millis());
}

void wakeupCallback() {
  ledTicker.attach(1, blinkLED);
  Serial.println("Waked up");
  Serial.flush();
}

void alarmOn(){
  ui.displayOn();
  alarmClock.disableAlarmEvent();
  alarmClock.playAlarm();
  Serial.println("Alarm on");
}
void alarmOff(){
  alarmClock.enableAlarmEvent();
  alarmClock.stopAlarm();
  Serial.println("Alarm off");
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
  tempSampleConnter++;
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
  ui.setDisplaySleepTime(10);
  ui.setContrast(5);
}

void tryConnectToWifi(void){
  ui.printStringAt(0,0 , "Connecting...");
  if(!wifiManger.autoConnect("Wifi-Clock")){
    ui.clearScreen(); 
    ui.printStringAt(0,0 , "Unable to connected", true);
    delay(2000);
    return;
  }
  ui.clearScreen(); 
  ui.printStringAt(0,0 , "Connected to " + wifiManger.getWiFiSSID(), true);
  delay(2000);    
}

void initWifiModule(void){
    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);
    disconnectedEvent = WiFi.onStationModeDisconnected(&onEspDisconnected);
    tryConnectToWifi();      
}

void turnWifiOff(void){
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

bool initNtpClient(void){
  ui.clearScreen();
  ui.printStringAt(0, 0, "Updating NTP", true);

  timeClient.begin();
  timeClient.setTimeOffset(12600);
  timeClient.setUpdateInterval(0);

  if(!timeClient.update()){
    ui.clearScreen();
    ui.printStringAt(0, 0, "NTP update failed", true);
    delay(2000);
    return false;
  }else{
    ui.clearScreen();
    ui.printStringAt(0, 0, "NTP update success", true);   
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

  ui.printStringAt(0, 0, "Setting RTC", true);
  if(!rtc.setEpoch(timeClient.getEpochTime())){
    ui.clearScreen();
    ui.printStringAt(0, 0, "RTC not found", true);
  }else{
    ui.clearScreen();
    ui.printStringAt(0, 0, "RTC set success", true);  
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

void lightSleep(){
  digitalWrite(LED_BUILTIN, 1);
  ledTicker.detach();
  Serial.println("Going to ligth sleep");

  extern os_timer_t *timer_list;
  timer_list = nullptr;
  wifi_set_opmode_current(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T); // set sleep type
  wifi_fpm_open(); // Enables force sleep
  gpio_pin_wakeup_enable(GPIO_ID_PIN(PIN_SW), GPIO_PIN_INTR_LOLEVEL); // GPIO_ID_PIN(7)
  wifi_fpm_set_wakeup_cb(wakeupCallback);
  wifi_fpm_do_sleep(0xFFFFFFF); // Sleep for longest possible time
  delay(10);

  gpio_pin_wakeup_disable();
  Serial.println("Continue");
}

void timedLightSleep(){
  digitalWrite(LED_BUILTIN, 1);
  ledTicker.detach();
  Serial.println("Going to timed light sleep");

  extern os_timer_t *timer_list;
  timer_list = nullptr;
  wifi_set_opmode_current(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  gpio_pin_wakeup_enable(GPIO_ID_PIN(PIN_SW), GPIO_PIN_INTR_LOLEVEL);
  wifi_fpm_set_wakeup_cb(wakeupCallback);
  wifi_fpm_do_sleep(oneMinuteSleep * 1000);
  delay(oneMinuteSleep + 100);
  
  gpio_pin_wakeup_disable();
  Serial.println("Continue");
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

float getTemprature(void){
  sensor.requestTemperatures();
  float tempC = sensor.getTempCByIndex(0);
  return tempC;
}

void showMainMenu(void){

  ui.disableSleepForDisplay();
  menu.setInputTime(millis());
  menu.resetMenu(5, 0);
  menuIdx = 0;
  enableRotaryMenuInterrupt();

  ui.clearScreen();
  ui.enableDefaultFont();

  while (true)
  { 
    ui.printAppBar(35,3, "Dashboard");
    if(menu.checkForAutoExit()) {
      disableRotaryMenuInterrupt();
      ui.enableSleepForDisplay();
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
            ui.printStringAt(10, 16, "Home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "FM radio", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "Clock set", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "Wifi set", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "Display", false);

            ui.updateScreen();

          if(menu.checkMenuSwitch() == CLICKED){
            disableRotaryMenuInterrupt();
            ui.enableSleepForDisplay();
            ui.clearScreen();
            return;
          }
        break;

      case FM_RADIO:
 
            ui.clearDisplayAt(1, 16, " ");
            ui.printStringAt(1, 26, "o");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "Home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "FM radio", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "Clock set", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "Wifi set", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "Display", false);
            ui.updateScreen();
        break;

      case CLOCK_SETTING:
 
            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.printStringAt(1, 36, "o");
            ui.clearDisplayAt(1, 46, " ");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "Home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "FM radio", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "Clock set", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "Wifi set", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "Display", false);

            ui.updateScreen();
            if(menu.checkMenuSwitch() == CLICKED) showClockSetting();
      
        break;

      case WIFI_SETTING:

            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.printStringAt(1, 46, "o");
            ui.clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "Home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "FM radio", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "Clock set", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "Wifi set", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "Display", false);

            ui.updateScreen();
            if(menu.checkMenuSwitch() == CLICKED) showWifiSetting();

      break;

      case DISPLAY:

            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.printStringAt(1, 56, "o");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "Home", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "FM radio", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "Clock set", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "Wifi set", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "Display", false);

            ui.updateScreen();
            if(menu.checkMenuSwitch() == CLICKED) showDisplaySetting();
 
        break;

      case EXIT:
 
            ui.clearDisplayAt(1, 16, " ");
            ui.clearDisplayAt(1, 26, " ");
            ui.clearDisplayAt(1, 36, " ");
            ui.clearDisplayAt(1, 46, " ");
            ui.printStringAt(1, 56, "o");

            display.fillRect(10, 16, 100, 8, BLACK);
            ui.printStringAt(10, 16, "FM radio", false);

            display.fillRect(10, 26, 100, 8, BLACK);
            ui.printStringAt(10, 26, "Clock set", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "Wifi set", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "Display", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "Exit", false);

            ui.updateScreen();

        if(menu.checkMenuSwitch() == CLICKED){
            disableRotaryMenuInterrupt();
            ui.clearScreen();
            ui.enableSleepForDisplay();
            return;
        }    
        break;
    }
  }
}

void showClockPage(){
  display.setTextColor(WHITE, BLACK);
  if(alarmClock.isAlarmOn()) ui.displayBell();
  
  while(1){

    /// get time from ds1307 rtc
    uint16 year;
    uint8_t hour, min, sec, temp, week;
    rtc.getDateTime(&hour, &min, &sec, &temp, &temp, &year, &week);

    if(++tempSampleConnter == TEMP_SAMPLE_TIME){
      tempSampleConnter = 0;
      tempC = getTemprature();
    }

    char clickStatus = menu.checkMenuSwitch();
    if(clickStatus == LONG_CLICKED){
      Serial.println("long");
      return;
    }
    if(clickStatus == CLICKED){
      if(alarmClock.isAlarmRunning()){
        alarmClock.stopAlarm();
      }
      ui.displayOn();
      if(alarmClock.isAlarmOn()) ui.displayBell();
      turnWifiOff();
    }
    if(ui.isDisplayTimeOut()){
      ui.displayOff();
      if(alarmClock.isAlarmOn()){
        timedLightSleep();
      }else{
        lightSleep();
      }
    }
    if(alarmClock.isAlarmOn()){
      alarmClock.tick(hour, min, sec);
      if(alarmClock.isAlarmRunning()){
        //alarmClock.alarmUpdate();
      }
    }

    ui.displayWeek(0, 0, 2, week);
    ui.displayHour(0,22,4, hour);
    ui.displayColon(44, 30,3);
    ui.displayMin(56,22,4, min);
    ui.displaySec(102, 35, 2, sec);
    ui.displayDate(0, 55, 1, ui.epochToDate(rtc.getEpoch()));
    ui.showBatteryPercentage(getBatteryLevel());
    ui.showTemprature(110, 55, 1, tempC);
    ui.updateScreen();    

    if(!ui.isDisplayOn()){
      timedLightSleep();
    }
  }
}

void showClockSetting(void){
  ui.clearScreen();
  menu.setMaxIndex(3);
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
      if(menu.checkMenuSwitch() == CLICKED) alarmSet();
      break;  
    
    case 3:
      showClockSettingTitles();
      ui.printStringAt(1, 16, " ");
      ui.clearDisplayAt(1, 26, " ");
      ui.clearDisplayAt(1, 36, " ");
      ui.clearDisplayAt(1, 46, "o");
      ui.updateScreen();
      if(menu.checkMenuSwitch() == CLICKED){
        menu.resetMenu(5, 2);
        menuIdx = 2;  
        ui.clearScreen();
        return;
      }
      break;  
    }
  }
}

void showClockSettingTitles(void){
  ui.printAppBar(35,3, "Clock Menu");
  ui.printStringAt(10, 16, "Time", false);
  ui.printStringAt(10, 26, "Date", false);
  ui.printStringAt(10, 36, "Alarm", false);
  ui.printStringAt(10, 46, "Exit", false);
}

void timeSet(void){
  ui.clearScreen();
  menu.resetMenu(2);

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
      menu.resetMenu(23, hour);
      menuIdx = hour;
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
      menu.resetMenu(59, min);
      menuIdx = min;
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
      menu.resetMenu(59, sec);
      menuIdx = sec;
      while (true)
      {
        sec = menuIdx;
        ui.displaySec(102, 35, 2, sec); 
        if(menu.checkMenuSwitch() == CLICKED){
          menu.setMaxIndex(3);
          menu.setCurrentIndex(0);
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
  menu.resetMenu(3);
  menuIdx = 0;  

  ui.printNumberAt(0, 25, 2, 12);
  ui.printStringAt(32, 25, 2, "Jun");
  ui.printStringAt(80, 25, 2, "2022");
  ui.printStringAt(32, 47, 2, "Wed");
  u_int8_t day, month, week;
  uint16_t year;

  while (true)
  {
    switch (menuIdx)
    {
    case 0:
      display.fillTriangle(5, 3, 25, 3, 15, 15, WHITE);
      menu.resetMenu(31, 1);
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
      menu.resetMenu(11, 1);
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
      menu.resetMenu(22, 22);
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
      menu.resetMenu(6, 0);
      menuIdx = 0;
      while (true)
      {
        week = menuIdx;
        if(menu.checkMenuSwitch() == CLICKED){
          menu.setMaxIndex(3);
          menu.setCurrentIndex(0);
          menuIdx = 0;
          ui.clearScreen();
          ui.enableDefaultFont();
          while(!rtc.setDate(day, month, year, week));
          return;
        } 
        ui.displayWeek(32, 47, 2, menuIdx);
        ui.updateScreen();  
      }
      break; 
    }
  }  
}

void alarmSet(void){

  uint8 alarmH, alarmM;

  ui.clearScreen();
  menu.resetMenu(2);
  menuIdx = 0;
  ui.printAppBar(35,3, "Alarm Menu"); 
  ui.displayHour(15,40,3, alarmClock.getHourAlarm());
  ui.displayColon(50, 40,3);
  ui.displayMin(70,40,3, alarmClock.getMinuteAlarm());

  while (true)
  {
    switch (menuIdx)
    {
      case 0:
        menu.resetMenu(1);
        if(alarmClock.isAlarmOn()) menuIdx = 1;
        else menuIdx = 0;
        while(true){
          if(menuIdx == 1){
            alarmClock.turnAlarm(ON);
            ui.printStringAt(40, 17, 2, "On ");    
          }else{
            alarmClock.turnAlarm(OFF);
            ui.printStringAt(40, 17, 2, "OFF"); 
          }
          if(menu.checkMenuSwitch() == CLICKED){
            menuIdx = 1;
            break;
          }  
          ui.updateScreen();
        }
        break;  

      case 1:
        menu.resetMenu(23, alarmClock.getHourAlarm());
        menuIdx = alarmClock.getHourAlarm();
        while(true){
          ui.displayHour(15,40,3, menuIdx);
          alarmH = menuIdx;
          if(menu.checkMenuSwitch() == CLICKED){
            menuIdx = 2;
            break;
          } 
          ui.updateScreen();
        }
        break;  

      case 2:
        menu.resetMenu(59, alarmClock.getMinuteAlarm());
        menuIdx = alarmClock.getMinuteAlarm();
        while(true){
          ui.displayMin(70,40,3, menuIdx);
          alarmM = menuIdx;
          if(menu.checkMenuSwitch() == CLICKED){
            alarmClock.setWhenAlarmOn(alarmH, alarmM, 0);
            alarmClock.setWhenAlarmOff(alarmH, alarmM + 1, 0);
            menu.resetMenu(3);
            menuIdx = 0;
            ui.clearScreen();
            ui.enableDefaultFont();
            return;
          } 
          ui.updateScreen();
        }
        break;  
    }
  }  
}

void showWifiSetting(void){
  ui.clearScreen();
  menu.resetMenu(1);
  menuIdx = 0;
  ui.printAppBar(35,3, "Wifi Menu");

  ui.printStringAt(1, 20, "o");
  ui.printStringAt(10, 37, 2, "T.Upd: no");
  ui.printStringAt(0, 55, 1, "<long press to exit>");

  while (true){
    switch (menuIdx){
      case 0:
        ui.printStringAt(10, 16, 2, "T.Ntp: !");
        while (true){
          ui.updateScreen();
          char clickStatus = menu.checkMenuSwitch();
          if(clickStatus == CLICKED){ 
            if(!wifiManger.autoConnect("Wifi-Clock")){
              menu.resetMenu(1, 0);
              menuIdx = 0;
              break;   
            }

            /// connet to ntp 
            timeClient.begin();
            timeClient.setTimeOffset(12600);
            timeClient.setUpdateInterval(0);
            if(!timeClient.update()){
              menu.resetMenu(1, 0);
              menuIdx = 0;
              break; 
            }

            /// update rtc
            rtc.setEpoch(timeClient.getEpochTime()); 
            turnWifiOff();
            ui.printStringAt(10, 16, 2, "T.Ntp: Ok");
            menu.resetMenu(1, 0);
            menuIdx = 1;
            break;
          } 
          if(clickStatus == LONG_CLICKED){
            menu.resetMenu(5, 3);
            menuIdx = 3; 
            ui.enableDefaultFont();
            ui.clearScreen();
            return; 
          }
        }  
        break;
    
      case 1:
        ui.printStringAt(1, 20, 1, " ");
        ui.printStringAt(1, 40, 1, "o");
        while (true){ 
          ui.updateScreen();
          if(menu.checkMenuSwitch() == CLICKED){
            menu.resetMenu(5, 3);
            menuIdx = 3;  
            ui.enableDefaultFont();
            ui.clearScreen();
            return;
          } 
        }
        break;
    }
  }  
}

void showDisplaySetting(void){

  ui.clearScreen();
  menu.resetMenu(1);
  menuIdx = 0;
  ui.printAppBar(30,3, "Display Menu");

  ui.printStringAt(1, 20, "o");
  ui.printStringAt(10, 16, 2, "T.Out:");
  ui.printStringAt(117, 20, 1, "S");

  ui.printStringAt(10, 37, 2, "D.Ctr:");
  ui.printNumberAt(90, 37, 2, 0, 3);

  while (true){
    switch (menuIdx){
    case 0:
      menu.resetMenu(60, 1, 1);
      menuIdx = 1;
      while (true){
        ui.printNumberAt(90, 16, 2, menuIdx, 2);
        ui.updateScreen();  
        if(menu.checkMenuSwitch() == CLICKED){
          ui.setDisplaySleepTime(menuIdx);
          menu.resetMenu(1, 0);
          menuIdx = 1;
          break;
        } 
      }  
      break;
    
    case 1:
      ui.printStringAt(1, 20, 1, " ");
      ui.printStringAt(1, 40, 1, "o");
      menu.resetMenu(255, 1, 1);
      while (true){ 
        ui.printNumberAt(90, 37, 2, menuIdx, 3);
        ui.setContrast(menuIdx);
        ui.updateScreen();
        if(menu.checkMenuSwitch() == CLICKED){
          menu.resetMenu(5, 2);
          menuIdx = 4;  
          ui.enableDefaultFont();
          ui.clearScreen();
          return;
        } 
      }
      break;
    }
  }  
}