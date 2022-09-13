#ifndef MyMenu_h
#define MyMenu_h

#include<RotaryEncoder.h>

class MyMenu{

    public:
        MyMenu(int pina, int pinb, int rotarySW);
        int getMenuIndex(void);
        void setMenuIndex(int index);

    private:
        int _menuIndex = 0;
        int _pin1, _pin2, _rotarySW;
        RotaryEncoder rotary = RotaryEncoder(0,0);
        int readRotEnc(void);

    
};

#endif