#include "src/myImports.h"

/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
void blinkLED();
void showWifiStatus(const char* text);
void initWifiModule(void);
void printCounter(u_char counter, int x, int y);



void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    initLcd();
    initWifiModule();

    /// start ticker
    ledTicker.attach(0.4, blinkLED);
}

void loop()
{
  counter++;
  printCounter(counter, 50, 20);
  checkWifiStatus();
  delay(1000);
}


////////// Implementaion methods  //////////

void blinkLED(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  if(conectedFlag) {
    ledTicker.attach(0.1, blinkLED);
  }
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

void printCounter(u_char counter, int x, int y){
  display.setTextSize(3);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(x, y);
  display.print("   ");
  display.setCursor(x, y);
  display.print(counter);
  display.display();
}
