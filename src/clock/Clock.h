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
        void displayColon(int x, int y, uint8_t size);
        void dislayWeek(int x, int y, uint8_t size, String week);
        void displayDate(int x, int y, uint8_t size, String date);
        void displayAmPm(int x, int y, uint8_t size, boolean isAM);
        void displayOff();
        void displayOn();
        void setDisplaySleepTime(uint8_t sec);
        uint8_t getDisplaySleepTime(void);
        void checkDisplaySleep(void);
        void resetDisplaySleepTimer(void);
        void enableDisplaySleep(void);
        void disableDisplaySleep(void);
        bool isEnableDisplaySleep(void);
        bool isDisplayTimeOut(void);

    private:
        Adafruit_SSD1306* _display;
        u_int8_t _displaySleepTime = 0;
        u_int8_t _displaySleepCounter = 0;
        bool _allowDisplaySleep = true;
        bool _displayTimeOut = false;
};


#endif