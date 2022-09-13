#include "src/myImports.h"

/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
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
    attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SW), checkMenuSwitch, ONLOW);

    /// start ticker
    ledTicker.attach(0.4, blinkLED);
}

IRAM_ATTR void checkPosition()
{
  menuIdx = menu.getMenuIndex();
}
IRAM_ATTR void checkMenuSwitch()
{
  menuIdx = 0;
  menu.setMenuIndex(0);
}

void loop()
{ 
  if(conectedFlag) printWifiStatus("Connected");
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
  pinMode(PIN_SW, INPUT_PULLUP);
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
    printWifiStatus("Connecting...");

    WiFiManager wifiManger;
    //wifiManger.erase(true);
    wifiManger.autoConnect("Wifi-Clock");
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

  display.setCursor(10, 16);
  display.print("home");
  display.display();

  display.setCursor(10, 26);
  display.print("clock settings");
  display.display();

  display.setCursor(10, 36);
  display.print("wifi settings");
  display.display();

  display.setCursor(10, 46);
  display.print("about system");
  display.display();

  while (1)
  {
    printCounter(menuIdx, 70, 16);
    switch (menuIdx) {
      case Home:
          printStringAt(1, 16, "o");
          clearDisplayAt(1, 26, " ");
          clearDisplayAt(1, 36, " ");
          clearDisplayAt(1, 46, " ");
        break;
      case Clock_SETTING:
          clearDisplayAt(1, 16, " ");
          printStringAt(1, 26, "o");
          clearDisplayAt(1, 36, " ");
          clearDisplayAt(1, 46, " ");
        break;
      case WIFI_SETTING:
          clearDisplayAt(1, 16, " ");
          clearDisplayAt(1, 26, " ");
          printStringAt(1, 36, "o");
          clearDisplayAt(1, 46, " ");
        break;
      case ABOUT_SYSTEM:
          clearDisplayAt(1, 16, " ");
          clearDisplayAt(1, 26, " ");
          clearDisplayAt(1, 36, " ");
          printStringAt(1, 46, "o");
        break;
    }
  }
  
  
}