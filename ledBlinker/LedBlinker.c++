#include "LedBlinker.h"
#include "Arduino.h"
#include <Ticker.h>

LedBlinker::LedBlinker(int pinNumber, float ticTime){

    pin = pinNumber;
    _tickTime = ticTime;
    pinMode(pinNumber, OUTPUT);    
}



void LedBlinker::startBlink(){
    int state = digitalRead(pin);
    digitalWrite(pin, !state);
}

void LedBlinker::initTicker(){
    LedBlinker::ticker.attach(_tickTime, startBlink);
}

void LedBlinker::stopBlink(){
    ticker.detach();
}