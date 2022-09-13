#include"MyMenu.h"


MyMenu::MyMenu(int pina, int pinb, int rotarySW){

    _pin1 = pina;
    _pin2 = pinb;
    _rotarySW = rotarySW;
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
    if(_menuIndex > 3)
        _menuIndex = 03;
  }
  return _menuIndex;
}    