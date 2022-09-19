#include "../definition.h"
#include"MyMenu.h"


MyMenu::MyMenu(int pina, int pinb, int rotarySW, int exitTimeMenu){

    _pin1 = pina;
    _pin2 = pinb;
    _rotarySW = rotarySW;
    _exitTime = exitTimeMenu;
    pinMode(_rotarySW, INPUT_PULLUP);

    this->rotary = RotaryEncoder(_pin1, _pin2);
    
}

int MyMenu::readRotEnc(){
    // returns -1 when turned left/down and +1 when turned right/up
    static int reLastPos = 0;
    int reCurrentPos;
    int reResult = 0;

    rotary.tick();
    reCurrentPos = rotary.getPosition();
    if (reCurrentPos < reLastPos)
        reResult = -1;
    else if (reCurrentPos > reLastPos)
        reResult = 1;

    reLastPos = reCurrentPos;
    return reResult;
}

void MyMenu::setMenuIndex(int index){
    this->_menuIndex = index;
}

int MyMenu::getMenuIndex(void) {
  int reResult = 0;
  reResult = readRotEnc();

  if (reResult < 0) {             // down
    --_menuIndex;
    if (_menuIndex < 0)
      _menuIndex = 0;
  } else if (reResult > 0) {      // up
    _menuIndex++;
    if(_menuIndex > 5)
        _menuIndex = 5;
  }
  return _menuIndex;
} 

int MyMenu::checkMenuSwitch(void){

    static int lastSwState = LOW;
    int swState = digitalRead(PIN_SW);

    if (swState == LOW && lastSwState == HIGH) {
        unsigned long startTime = millis();
        
        while (digitalRead(PIN_SW) == LOW) {                                    
            lastSwState = swState;
            if (millis() - startTime > 1000)
                break;
        }
        
        if (millis() - startTime > 1000)
            return LONG_CLICKED;
        else
            return CLICKED;
    }
  
    lastSwState = swState; 
    return false;   
}

int MyMenu::getMenuRpm(){
    return this->rotary.getRPM();
}

void MyMenu::setInputTime(int time){
    _inputTime = time;
}

bool MyMenu::checkForAutoExit(void){
    int timePassed = (millis() - _inputTime) / 1000;
    if(timePassed > _exitTime) {
        setInputTime(0);
        return true;
    }else{
        return false;
    }
}