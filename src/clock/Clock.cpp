#include"Clock.h"

Clock::Clock(void){
    _display = nullptr;
}

Clock::Clock(Adafruit_SSD1306* display){
    _display = display;
}

void Clock::displayOff(){
     _display->ssd1306_command(SSD1306_DISPLAYOFF);
}

void Clock::displayOn(){
    _display->ssd1306_command(SSD1306_DISPLAYON);
}

void Clock::displaySec(int x, int y, uint8_t size, String sec){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(sec);
}

void Clock::displayMin(int x, int y, uint8_t size, String min){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(min);    
}

void Clock::displayHour(int x, int y, uint8_t size, String hour){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(hour);     
}

void Clock::displayColon(int x, int y, uint8_t size){
     _display->setTextSize(size);
     _display->setCursor(x, y);
     _display->print(":");
}

void Clock::dislayWeek(int x, int y, uint8_t size, String week){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(week);
}

void Clock::displayDate(int x, int y, uint8_t size, String date){
    _display->setCursor(x, y);
    _display->setTextSize(size);
    _display->print(date);
}

void Clock::displayAmPm(int x, int y, uint8_t size, boolean isAM){
    _display->setCursor(x, y);
    _display->setTextSize(size);
    if(isAM){
        _display->print("AM");
    }else{
        _display->print("PM");
    }    
}

void Clock::setDisplaySleepTime(uint8_t sec){
    _displaySleepTime = sec;
}

uint8_t Clock::getDisplaySleepTime(void){
    return _displaySleepTime;
}

void Clock::resetDisplaySleepTimer(void){
    _displaySleepCounter = 0;
}

void Clock::checkDisplaySleep(void){
    if(isEnableDisplaySleep()){
        _displaySleepCounter++;
        if(getDisplaySleepTime() == _displaySleepCounter) {
            displayOff();
            resetDisplaySleepTimer();
            disableDisplaySleep();
        }
    }
}

void Clock::enableDisplaySleep(void){
    _allowDisplaySleep = true;
}

void Clock::disableDisplaySleep(void){
    _allowDisplaySleep = false;
}

char Clock::isEnableDisplaySleep(void){
    if(_allowDisplaySleep)
        return 1;
    else 
        return 0;
}