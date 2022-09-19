#include "definition.h"

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

/// rotary switch
#include "menu/MyMenu.h"

/// display clock
#include"clock/Clock.h"

/// fonts
#include"customFonts/orbitron_9.h"
#include<Fonts/FreeMonoOblique9pt7b.h>


WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;
Adafruit_SSD1306 display;

TwoWire wire;

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;

/// menu selector
MyMenu menu(PIN_ROTARY_IN1, PIN_ROTARY_IN2, PIN_SW, 10);
int menuIdx = 0;

/// clock
Clock myClock;

