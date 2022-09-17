#ifndef Clock_h
#define Clock_h

#include<Adafruit_SSD1306.h>

class Clock{

    public:
        Clock();
        Clock(Adafruit_SSD1306* display);
        void displaySec(int x, int y, uint8_t size, String sec);
        void displayMin(int x, int y, uint8_t size, String min);
        void displayHour(int x, int y, uint8_t size, String hour);
        void displayOff();
        void displayOn();

    private:
        Adafruit_SSD1306* _display;
};


#endif