/// for wifi module
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

/// display
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/// commons
#include <Ticker.h>
#include <stdio.h>

/// Lcd pins
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;
Adafruit_SSD1306 display;

TwoWire wire;

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;

/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
void blinkLED();
void showWifiStatus(const char* text);
void initWifiModule(void);




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
  checkWifiStatus();
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
