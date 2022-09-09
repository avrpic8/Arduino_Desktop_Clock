#include <Arduino.h>
#line 1 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <stdio.h>


const char* ssid = "ARYA";
const char* password = "ehsansaeed18041543";

WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;


/// function definition
void initWifiModule(void);



#line 23 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
void setup();
#line 34 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
void loop();
#line 39 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
void onEspConnected(const WiFiEventStationModeConnected& event);
#line 45 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
void blinkLED();
#line 23 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    initWifiModule();
	
    /// start ticker
    ledTicker.attach(0.4, blinkLED);
}

void loop()
{

}

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

void initWifiModule(void){
  /// setup wifi mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);


    WiFiManager wifiManger;
    wifiManger.autoConnect("Wifi-Clock");
}

