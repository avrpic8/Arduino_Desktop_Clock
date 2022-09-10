/// for wifi module
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

/// display
#include <Wire.h>
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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;

const char* ssid = "ARYA";
const char* password = "ehsansaeed18041543";


/// function definition
void onEspConnected(const WiFiEventStationModeConnected& event);
void blinkLED();
void showWifiStatus(const char* text);
void initWifiModule(void);




void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    display.clearDisplay();
    delay(2000);
  
    initWifiModule();

    /// start ticker
    ledTicker.attach(0.4, blinkLED);
}

void loop()
{
  if(conectedFlag) showWifiStatus("Conected!");
}


////////// Implementaion methods  //////////

void onEspConnected(const WiFiEventStationModeConnected& event){
    Serial.print("connected to the wifi: ");
    Serial.println(event.ssid);
    conectedFlag = true;     
}

void blinkLED(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  if(conectedFlag) {
    ledTicker.attach(0.1, blinkLED);
  }
}

void showWifiStatus(const char* text){
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}

void initWifiModule(void){
    WiFi.mode(WIFI_STA);

    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);
    showWifiStatus("Connecting...");

    WiFiManager wifiManger;
    wifiManger.autoConnect("Wifi-Clock");
}
