#include"Ui.h"

Ui::Ui(void){
    _display = nullptr;
}

Ui::Ui(Adafruit_SSD1306* display){
    _display = display;
}

/// methods for display clock
void Ui::displaySec(int x, int y, uint8_t size, String sec){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(sec);
}

void Ui::displayMin(int x, int y, uint8_t size, String min){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(min);    
}

void Ui::displayHour(int x, int y, uint8_t size, String hour){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(hour);     
}

void Ui::displayColon(int x, int y, uint8_t size){
     _display->setTextSize(size);
     _display->setCursor(x, y);
     _display->print(":");
}

void Ui::dislayWeek(int x, int y, uint8_t size, String week){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(week);
}

void Ui::displayDate(int x, int y, uint8_t size, String date){
    _display->setCursor(x, y);
    _display->setTextSize(size);
    _display->print(date);
}

void Ui::displayAmPm(int x, int y, uint8_t size, boolean isAM){
    _display->setCursor(x, y);
    _display->setTextSize(size);
    if(isAM){
        _display->print("AM");
    }else{
        _display->print("PM");
    }    
}


/// methods for control ssd1306 sleep and wakup
void Ui::displayOff(){

    /// clear horizantal
    for(int16_t col=0; col<33; col++){
        _display->drawLine(0, col, 127, col, BLACK);
        _display->drawLine(0, 63 - col, 127, 63 - col, BLACK);
        updateScreen();
    }

    _display->drawLine(0, 33, 127, 33, WHITE);

    /// clear vertical
    for(int16_t row = 0; row < 63; row++){
        _display->drawPixel(row, 33, BLACK);
        _display->drawPixel(127 - row, 33, BLACK);
        updateScreen();
    }

    _displayTimeOut = false;
    _display->ssd1306_command(SSD1306_DISPLAYOFF);
}

void Ui::displayOn(){
    enableDisplaySleep();
    _display->ssd1306_command(SSD1306_DISPLAYON);   
}

void Ui::setDisplaySleepTime(uint8_t sec){
    _displaySleepTime = sec;
}

uint8_t Ui::getDisplaySleepTime(void){
    return _displaySleepTime;
}

void Ui::resetDisplaySleepTimer(void){
    _displaySleepCounter = 0;
}

void Ui::checkDisplaySleep(void){
    if(isEnableDisplaySleep()){
        _displaySleepCounter++;
        if(getDisplaySleepTime() == _displaySleepCounter) {
            resetDisplaySleepTimer();
            disableDisplaySleep();    
        }
    }
}

void Ui::enableDisplaySleep(void){
    _allowDisplaySleep = true;
    _displayTimeOut = false;
}

void Ui::disableDisplaySleep(void){
    _allowDisplaySleep = false;
    _displayTimeOut = true;
}

bool Ui::isEnableDisplaySleep(void){
    if(_allowDisplaySleep)
        return true;
    else 
        return false;
}

bool Ui::isDisplayTimeOut(void){
    return _displayTimeOut;
}


/// simple helper functins for print on screen
void Ui::clearScreen(void){
    _display->clearDisplay();
}

void Ui::updateScreen(void){
    _display->display();
}

void Ui::clearDisplayAt(int x, int y, String len){
    _display->setTextColor(WHITE,BLACK);
    _display->setCursor(x, y);
    _display->print(len); 
    _display->display(); 
}

void Ui::printStringAt(int x, int y, String message){
    _display->setTextColor(WHITE,BLACK);
    _display->setCursor(x, y);
    _display->print(message); 
    _display->display(); 
}

void Ui::printStringAt(int x, int y, String message, bool update){
    _display->setTextColor(WHITE,BLACK);
    _display->setCursor(x, y);
    _display->print(message); 
    if(update)
        _display->display();     
}

void Ui::printAppBar(int x, int y, String title){
    _display->fillRect(0,0,126,15,WHITE);
    _display->setCursor(x, y);
    _display->setTextSize(1);
    _display->setTextColor(BLACK);
    _display->print(title);
}

void Ui::enableDefaultFont(){
    _display->setFont();
    _display->setTextSize(1);
}

void Ui::setContrast(uint8_t value){
    _display->ssd1306_command(SSD1306_SETCONTRAST);
    _display->ssd1306_command(value);
}