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


u_char counter = 0;

