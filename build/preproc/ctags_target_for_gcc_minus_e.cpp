# 1 "/home/saeed/Smart Electronics/Arduino/Wifi_tutorial/wifi_events.ino"
# 2 "/home/saeed/Smart Electronics/Arduino/Wifi_tutorial/wifi_events.ino" 2
# 3 "/home/saeed/Smart Electronics/Arduino/Wifi_tutorial/wifi_events.ino" 2
# 4 "/home/saeed/Smart Electronics/Arduino/Wifi_tutorial/wifi_events.ino" 2



# 6 "/home/saeed/Smart Electronics/Arduino/Wifi_tutorial/wifi_events.ino"
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
    pinMode(2, 0x01);

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
  int state = digitalRead(2);
  digitalWrite(2, !state);
  if(conectedFlag) {
    ledTicker.attach(0.1, blinkLED);
  }
}
