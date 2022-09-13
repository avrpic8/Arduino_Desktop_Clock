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


WiFiEventHandler connectedEvent;
WiFiEventHandler disconnectedEvent;
Adafruit_SSD1306 display;

TwoWire wire;

/// ticker 
Ticker ledTicker;
bool conectedFlag = false;

/// menu selector
MyMenu menu(PIN_IN1, PIN_IN2, PIN_SW);
int menuIdx = 0;

