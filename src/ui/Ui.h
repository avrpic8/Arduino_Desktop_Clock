#ifndef Ui_h
#define Ui_h

#include<Adafruit_SSD1306.h>

class Ui{

    public:
        Ui();
        Ui(Adafruit_SSD1306* display);

        /// methods for display clock
        void displaySec(int x, int y, uint8_t size, int sec);
        void displayMin(int x, int y, uint8_t size, int min);
        void displayHour(int x, int y, uint8_t size, int hour);
        void displayColon(int x, int y, uint8_t size);
        void displayWeek(int x, int y, uint8_t size, int week);
        void displayDate(int x, int y, uint8_t size, String date);
        void displayMonth(int x, int y, uint8_t size, int num);
        void displayAmPm(int x, int y, uint8_t size, boolean isAM);
        String epochToDate(time_t epoch);


        /// methods for control ssd1306 sleep and wakup
        void displayOff();
        void displayOn();
        void setDisplaySleepTime(uint8_t sec);
        uint8_t getDisplaySleepTime(void);
        void checkDisplaySleep(void);
        void resetDisplaySleepTimer(void);
        void enableSleepForDisplay(void);
        void disableSleepForDisplay(void);
        bool isDisplayTimeOut(void);
        bool isDisplayOn(void);


        /// simple helper functins for print on screen
        void clearScreen(void);
        void updateScreen(void);
        void clearDisplayAt(int x, int y, String len);
        void printStringAt(int x, int y, String message);
        void printStringAt(int x, int y, int textSize, String message);
        void printStringAt(int x, int y, String message, bool update);
        void printNumberAt(int x, int y, int size, int num);
        void printNumberAt(int x, int y, int size, int num, int lenght);
        void printAppBar(int x, int y, String title);
        void showBatteryPercentage(char value);
        void showTemprature(int x, int y, int size, char value);
        void enableDefaultFont();
        void setContrast(uint8_t value);

    private:
        Adafruit_SSD1306* _display;
        u_int8_t _displaySleepTime = 0;
        u_int8_t _displaySleepCounter = 0;
        bool _allowDisplaySleep = true;
        bool _displayTimeOut = false;
        bool _displayOn = true;

        //Week Days
        String _weekDays[7]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

        //Month names
        String _months[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
};


#endif