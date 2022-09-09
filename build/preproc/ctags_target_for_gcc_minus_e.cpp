# 1 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
# 2 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino" 2
# 3 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino" 2
# 4 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino" 2
# 5 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino" 2



# 7 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/wifi_events.ino"
const char* ssid = "ARYA";
const char* password = "ehsansaeed18041543";

WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;


/// function definition
void initWifiModule(void);



void setup()
{
    Serial.begin(115200);
    pinMode(2, 0x01);

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
  int state = digitalRead(2);
  digitalWrite(2, !state);
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
    //wifiManger.autoConnect("Wifi-Clock");
}
