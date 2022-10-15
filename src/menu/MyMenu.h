#ifndef MyMenu_h
#define MyMenu_h

#include<RotaryEncoder.h>

class MyMenu{

    public:
        MyMenu(int pina, int pinb, int rotarySW, int exitTimeMenu);
        int getMenuIndex(void);
        void setMaxIndex(int maxMargin);
        void setCurrentIndex(int value);
        int getMaxIndex(void);
        int checkMenuSwitch();
        int getMenuRpm();
        bool checkForAutoExit(void);
        void setInputTime(int time);
        void resetMenu(int maxIndex);
        void resetMenu(int maxIndex, int currentIndex);

    private:
        int _menuIndex = 0;
        int _maxMargin = 0;
        int _pin1, _pin2, _rotarySW;
        u_int8_t _exitTime = 0;
        int _inputTime = 0;
        RotaryEncoder rotary = RotaryEncoder(0,0);

        int readRotEnc(void);  
};

#endif