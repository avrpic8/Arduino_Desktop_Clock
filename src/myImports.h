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
#include <Adafruit_AHTX0.h>
#include <ErriezDS1307.h>
#include <Ticker.h>
#include <stdio.h>
#include "menu/MyMenu.h"
#include"ui/Ui.h"
#include"alarm/Alarm.h"

/// radio
#include <radio.h>
#include <RDA5807M.h>

/// fonts and icons
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
Alarm myTimer;


/// ntp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, 0, 60000);


/// rtc
ErriezDS1307 rtc;
uint32 rtcUpdateCounter = 0;
char minuteCounter =0, secCounter = 0;  /// int timer page
bool allowCountDown = false;


/// Aht10 Humidity and temerature sensor
Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;

char sensorSampleConnter = 0;
bool countNow = true;
bool allowSamplingSensors = true;
bool tourchState = false;


/// radio object
RDA5807M radio;  