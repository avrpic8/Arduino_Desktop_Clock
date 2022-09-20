#ifndef Ui_h
#define Ui_h

#include<Adafruit_SSD1306.h>

class Ui{

    public:
        Ui();
        Ui(Adafruit_SSD1306* display);

        /// methods for display clock
        void displaySec(int x, int y, uint8_t size, String sec);
        void displayMin(int x, int y, uint8_t size, String min);
        void displayHour(int x, int y, uint8_t size, String hour);
        void displayColon(int x, int y, uint8_t size);
        void dislayWeek(int x, int y, uint8_t size, String week);
        void displayDate(int x, int y, uint8_t size, String date);
        void displayAmPm(int x, int y, uint8_t size, boolean isAM);


        /// methods for control ssd1306 sleep and wakup
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


        /// simple helper functins for print on screen
        void clearScreen(void);
        void updateScreen(void);
        void clearDisplayAt(int x, int y, String len);
        void printStringAt(int x, int y, String message);
        void printStringAt(int x, int y, String message, bool update);
        void printAppBar(int x, int y, String title);
        void enableDefaultFont();
        void setContrast(uint8_t value);

    private:
        Adafruit_SSD1306* _display;
        u_int8_t _displaySleepTime = 0;
        u_int8_t _displaySleepCounter = 0;
        bool _allowDisplaySleep = true;
        bool _displayTimeOut = false;
};


#endif