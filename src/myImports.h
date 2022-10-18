#include "definition.h"

/// Gpio
extern "C" {
   #include "gpio.h"
 }
extern "C" {
   #include "user_interface.h"
 }


/// for wifi module
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ErriezDS1307.h>
#include <Ticker.h>
#include <stdio.h>
#include "menu/MyMenu.h"
#include"ui/Ui.h"
#include"alarm/Alarm.h"

/// onewire
#include <OneWire.h>
#include <DallasTemperature.h>

/// fonts
#include"customFonts/orbitron_9.h"
#include<Fonts/FreeMonoOblique9pt7b.h>

WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;
Adafruit_SSD1306 display;
TwoWire wire;


/// wifi manager
WiFiManager wifiManger;
long oneMinuteSleep = 60e3;  /// one minutes in sleep


/// ticker 
Ticker ledTicker;
bool conectedFlag = false;


/// menu selector
MyMenu menu(PIN_ROTARY_IN1, PIN_ROTARY_IN2, PIN_SW, 10);
int menuIdx = 0;


/// ui manager
Ui ui;


/// alarm clock
Alarm alarmClock;


/// ntp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, 0, 60000);


/// rtc
ErriezDS1307 rtc;
uint32 rtcUpdateCounter = 0;


/// oneWire ds18b20
OneWire onewire(ONEWIRE_BUS);
DallasTemperature sensor(&onewire);
char tempC = 0;
char tempSampleConnter = 0;