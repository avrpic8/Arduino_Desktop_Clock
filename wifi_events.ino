#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <stdio.h>


const char* ssid = "ARYA";
const char* password = "ehsansaeed18041543";

WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
	
    /// setup wifi mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    /// register events
    connectedEvent = WiFi.onStationModeConnected(&onEspConnected);

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
