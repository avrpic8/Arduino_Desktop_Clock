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
    _display->fillRect(x, y-20, 50,30, BLACK);
    _display->setCursor(x, y);
    _display->print(sec);
}

void Clock::displayMin(int x, int y, uint8_t size, String min){
    _display->setTextSize(size);
    _display->fillRect(x, y-20, 50,50, BLACK);
    _display->setCursor(x, y);
    _display->print(min);    
}

void Clock::displayHour(int x, int y, uint8_t size, String hour){
    _display->setTextSize(size);
    _display->fillRect(x, y-20, 50,60, BLACK);
    _display->setCursor(x, y);
    _display->print(hour);     
}