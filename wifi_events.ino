#include "src/myImports.h"


/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
void onEspDisconnected(const WiFiEventStationModeDisconnected& event);
void blinkLED();
void intiPins(void);
void initWifiModule(void);
void enableRotaryMenuInterrupt(void);
void disableRotaryMenuInterrupt(void);
void showMainMenu(void);
void showClockPage(void);

void setup()
{
    Serial.begin(115200);
    
    initLcd();
    intiPins();
    initNtpClient();
    initWifiModule();
    
    /// start ticker
    ledTicker.attach(1, blinkLED);

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

void initLcd(void){
  wire.begin(SDA,SCL);
  wire.setClock(1000000);
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire, -1);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
  }
  display.clearDisplay();
  
  ui = Ui(&display);
  ui.setDisplaySleepTime(10);
  ui.setContrast(1);
}

void initWifiModule(void){
    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);
    disconnectedEvent = WiFi.onStationModeDisconnected(&onEspDisconnected);

    ui.printStringAt(0,0 , "Connecting...");
    WiFiManager wifiManger;
    //wifiManger.erase(true);
    wifiManger.autoConnect("Wifi-Clock");
    ui.printStringAt(0,0 , "Connected to " + wifiManger.getWiFiSSID());
    delay(3000);
}

void initNtpClient(void){
  timeClient.begin();
  timeClient.setTimeOffset(16200);
  timeClient.setUpdateInterval(60000);
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

void showMainMenu(void){

  menuIdx = 0;
  ui.disableDisplaySleep();
  menu.setInputTime(millis());
  enableRotaryMenuInterrupt();

  ui.clearScreen();
  ui.enableDefaultFont();
  ui.printAppBar(35,3, "Dashboard");

  while (true)
  { 
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
            ui.printStringAt(10, 26, "clock settings", false);

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
            ui.printStringAt(10, 26, "clock settings", false);

            display.fillRect(10, 36, 100, 8, BLACK);
            ui.printStringAt(10, 36, "wifi settings", false);

            display.fillRect(10, 46, 100, 8, BLACK);
            ui.printStringAt(10, 46, "system", false);

            display.fillRect(10, 56, 100, 8, BLACK);
            ui.printStringAt(10, 56, "display", false);

            ui.updateScreen();
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
            ui.printStringAt(10, 26, "clock settings", false);

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
            ui.printStringAt(10, 26, "clock settings", false);

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
            ui.printStringAt(10, 26, "clock settings", false);

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
            ui.printStringAt(10, 16, "clock settings", false);

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

    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    String timeFormat = timeClient.getFormattedTime();
    
    ui.dislayWeek(0, 0, 2, timeClient.getDay());
    ui.displayHour(0,22,4, timeFormat.substring(0, 2));
    ui.displayColon(44, 30,3);
    ui.displayMin(56,22,4,timeFormat.substring(3, 5));
    ui.displaySec(102, 35, 2, timeFormat.substring(6, 8));
    ui.displayDate(0, 55, 1, ui.epochToDate(epochTime));
    ui.updateScreen();  
  }
}