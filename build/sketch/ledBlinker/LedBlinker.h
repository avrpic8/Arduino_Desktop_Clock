#line 1 "/home/Saeed/Smart Electronics/Arduino/Arduino_wifi/ledBlinker/LedBlinker.h"
#ifndef LedBlinker_h
#define LedBlinker_h
#include "Arduino.h"
#include <Ticker.h>

class LedBlinker
{
private:
    int pin;
    float _tickTime;
public:
    LedBlinker(int pinNumber, float tickTime);
    ~LedBlinker();

    /// variebles
    Ticker ticker;

    /// methods
    void initTicker(void);
    void startBlink(void);
    void stopBlink();
};

#endif
