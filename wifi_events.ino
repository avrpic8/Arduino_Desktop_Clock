#include "src/myImports.h"



/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
void onEspDisconnected(const WiFiEventStationModeDisconnected& event);
void blinkLED();
void intiPins(void);
void printWifiStatus(const char* text);
void initWifiModule(void);
void printCounter(u_char counter, int x, int y);
void clearDisplayAt(int x, int y, String len);
void printStringAt(int x, int y, String message);
void showMainMenu(void);

void setup()
{
    Serial.begin(115200);
    
    initLcd();
    intiPins();
    initWifiModule();


    // register interrupt routine
    //attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_IN1), checkPosition, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_IN2), checkPosition, CHANGE);

    /// start ticker
    ledTicker.attach(0.4, blinkLED);
}

IRAM_ATTR void checkPosition()
{
  Serial.println("interrupt");
  menuIdx = menu.getMenuIndex();
  shouldRefresh = true;
}

void loop()
{ 
  checkWifiStatus();

  do{
    //menuIdx = 0;
    printStringAt(50, 20, "Test");
    display.fillRect(01, 16, 100, 8, BLACK);
    //display.display();
  }while(menu.checkMenuSwitch() != LONG_CLICKED);
  showMainMenu();
}


////////// Implementaion methods  //////////

void blinkLED(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  if(conectedFlag) {
    ledTicker.attach(0.1, blinkLED);
  }
}

void intiPins(void){
  pinMode(LED_BUILTIN, OUTPUT);
}

void initLcd(void){
  wire.begin(SDA,SCL);
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire, -1);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
  }
  display.clearDisplay();
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(5);
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

void printWifiStatus(const char* text){
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.print("             ");
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}

void checkWifiStatus(){
  if(conectedFlag) {
    printWifiStatus("Conected!");
  }else{
    printWifiStatus("Conecting...");
  }
}

void initWifiModule(void){
    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);
    disconnectedEvent = WiFi.onStationModeDisconnected(&onEspDisconnected);

    printWifiStatus("Connecting...");

    //WiFiManager wifiManger;
    //wifiManger.erase(true);
    //wifiManger.autoConnect("Wifi-Clock");
}

void printCounter(int counter, int x, int y){
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(x, y);
  display.print("   ");
  display.setCursor(x, y);
  display.print(counter);
  display.display();
}

void clearDisplayAt(int x, int y, String len){
  display.setTextColor(WHITE,BLACK);
  display.setCursor(x, y);
  display.print(len); 
  display.display(); 
}

void printStringAt(int x, int y, String message){
  display.setTextColor(WHITE,BLACK);
  display.setCursor(x, y);
  display.print(message); 
  display.display(); 
}

void showMainMenu(void){

  attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_IN2), checkPosition, CHANGE);

  display.clearDisplay();
  printStringAt(0,0, "Dashboard");

  display.setCursor(10, 16);
  display.print("home");

  display.setCursor(10, 26);
  display.print("clock settings");

  display.setCursor(10, 36);
  display.print("wifi settings");

  display.setCursor(10, 46);
  display.print("system");

  display.setCursor(10, 56);
  display.print("display");

  while (1)
  {
    switch (menuIdx) {
      case Home:
            printStringAt(1, 16, "o");
            clearDisplayAt(1, 26, " ");
            clearDisplayAt(1, 36, " ");
            clearDisplayAt(1, 46, " ");
            clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            display.setCursor(10, 16);
            display.print("home");

            display.fillRect(10, 26, 100, 8, BLACK);
            display.setCursor(10, 26);
            display.print("clock settings");

            display.fillRect(10, 36, 100, 8, BLACK);
            display.setCursor(10, 36);
            display.print("wifi settings");

            display.fillRect(10, 46, 100, 8, BLACK);
            display.setCursor(10, 46);
            display.print("system");

            display.fillRect(10, 56, 100, 8, BLACK);
            display.setCursor(10, 56);
            display.print("display");

            display.display();
            shouldRefresh = false;
          
          if(menu.checkMenuSwitch() == CLICKED){
            detachInterrupt(PIN_ROTARY_IN1);
            detachInterrupt(PIN_ROTARY_IN2);
            display.clearDisplay();
            return;
          }
        break;

      case Clock_SETTING:
 
            clearDisplayAt(1, 16, " ");
            printStringAt(1, 26, "o");
            clearDisplayAt(1, 36, " ");
            clearDisplayAt(1, 46, " ");
            clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            display.setCursor(10, 16);
            display.print("home");

            display.fillRect(10, 26, 100, 8, BLACK);
            display.setCursor(10, 26);
            display.print("clock settings");

            display.fillRect(10, 36, 100, 8, BLACK);
            display.setCursor(10, 36);
            display.print("wifi settings");

            display.fillRect(10, 46, 100, 8, BLACK);
            display.setCursor(10, 46);
            display.print("system");

            display.fillRect(10, 56, 100, 8, BLACK);
            display.setCursor(10, 56);
            display.print("display");

            display.display();
            shouldRefresh = false;
          
        break;

      case WIFI_SETTING:
 
            clearDisplayAt(1, 16, " ");
            clearDisplayAt(1, 26, " ");
            printStringAt(1, 36, "o");
            clearDisplayAt(1, 46, " ");
            clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            display.setCursor(10, 16);
            display.print("home");

            display.fillRect(10, 26, 100, 8, BLACK);
            display.setCursor(10, 26);
            display.print("clock settings");

            display.fillRect(10, 36, 100, 8, BLACK);
            display.setCursor(10, 36);
            display.print("wifi settings");

            display.fillRect(10, 46, 100, 8, BLACK);
            display.setCursor(10, 46);
            display.print("system");

            display.fillRect(10, 56, 100, 8, BLACK);
            display.setCursor(10, 56);
            display.print("display");

            display.display();
            shouldRefresh = false;
      
        break;

      case SYSTEM:

            clearDisplayAt(1, 16, " ");
            clearDisplayAt(1, 26, " ");
            clearDisplayAt(1, 36, " ");
            printStringAt(1, 46, "o");
            clearDisplayAt(1, 56, " ");

            display.fillRect(10, 16, 100, 8, BLACK);
            display.setCursor(10, 16);
            display.print("home");

            display.fillRect(10, 26, 100, 8, BLACK);
            display.setCursor(10, 26);
            display.print("clock settings");

            display.fillRect(10, 36, 100, 8, BLACK);
            display.setCursor(10, 36);
            display.print("wifi settings");

            display.fillRect(10, 46, 100, 8, BLACK);
            display.setCursor(10, 46);
            display.print("system");

            display.fillRect(10, 56, 100, 8, BLACK);
            display.setCursor(10, 56);
            display.print("display");

            display.display();
            shouldRefresh = false;

      break;

      case DISPLAY:

            clearDisplayAt(1, 16, " ");
            clearDisplayAt(1, 26, " ");
            clearDisplayAt(1, 36, " ");
            clearDisplayAt(1, 46, " ");
            printStringAt(1, 56, "o");

            display.fillRect(10, 16, 100, 8, BLACK);
            display.setCursor(10, 16);
            display.print("home");

            display.fillRect(10, 26, 100, 8, BLACK);
            display.setCursor(10, 26);
            display.print("clock settings");

            display.fillRect(10, 36, 100, 8, BLACK);
            display.setCursor(10, 36);
            display.print("wifi settings");

            display.fillRect(10, 46, 100, 8, BLACK);
            display.setCursor(10, 46);
            display.print("system");

            display.fillRect(10, 56, 100, 8, BLACK);
            display.setCursor(10, 56);
            display.print("display");

            display.display();
            shouldRefresh = false;
 
        break;

      case EXIT:
 
            clearDisplayAt(1, 16, " ");
            clearDisplayAt(1, 26, " ");
            clearDisplayAt(1, 36, " ");
            clearDisplayAt(1, 46, " ");
            printStringAt(1, 56, "o");

            display.fillRect(10, 16, 100, 8, BLACK);
            display.setCursor(10, 16);
            display.print("clock settings");

            display.fillRect(10, 26, 100, 8, BLACK);
            display.setCursor(10, 26);
            display.print("wifi settings");

            display.fillRect(10, 36, 100, 8, BLACK);
            display.setCursor(10, 36);
            display.print("system");

            display.fillRect(10, 46, 100, 8, BLACK);
            display.setCursor(10, 46);
            display.print("display");

            display.fillRect(10, 56, 100, 8, BLACK);
            display.setCursor(10, 56);
            display.print("exit");

            display.display();
            shouldRefresh = false; 

        if(menu.checkMenuSwitch() == CLICKED){
            detachInterrupt(PIN_ROTARY_IN1);
            detachInterrupt(PIN_ROTARY_IN2);
            display.clearDisplay();
            return;
        }    
        break;
    }
  }
}