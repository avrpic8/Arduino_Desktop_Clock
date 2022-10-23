#include"Ui.h"

Ui::Ui(void){
    _display = nullptr;
}

Ui::Ui(Adafruit_SSD1306* display){
    _display = display;
}

/// methods for display clock
void Ui::displaySec(int x, int y, uint8_t size, int sec){
    char buff[2];
    sprintf(buff, "%02d", sec);
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(buff);
}

void Ui::displayMin(int x, int y, uint8_t size, int min){
    char buff[2];
    sprintf(buff, "%02d", min);
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(buff);    
}

void Ui::displayHour(int x, int y, uint8_t size, int hour){
    char buff[2];
    sprintf(buff, "%02d", hour);
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(buff);     
}

void Ui::displayColon(int x, int y, uint8_t size){
     _display->setTextSize(size);
     _display->setCursor(x, y);
     _display->print(":");
}

void Ui::displayWeek(int x, int y, uint8_t size, int week){
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(_weekDays[week]);
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

void Ui::displayMonth(int x, int y, uint8_t size, int num){
    _display->setCursor(x, y);
    _display->setTextSize(size);
    _display->print(_months[num]);    
}

String Ui::epochToDate(time_t epoch){
    struct tm *ptm = gmtime ((time_t *)&epoch); 
    int monthDay     = ptm->tm_mday;
    int currentMonth = ptm->tm_mon + 1;
    String currentMonthName = _months[currentMonth-1];
    int currentYear  = ptm->tm_year + 1900;
    String currentDate =  String(monthDay) +  " " + String(currentMonthName) +  " " + String(currentYear);
    return currentDate;
}

void Ui::displayBell(void){
    _display->drawBitmap(60,0, siren_icon16x16, 16,16, 1);
}

void Ui::displayTourchOn(void){
    _display->drawBitmap(50,25, tourch_on32x16, 28,16, 1); 
}

void Ui::displayTourchOff(void){
    _display->drawBitmap(50,25, tourch_off32x16, 28,16, 1); 
}

void Ui::checkLightState(int state){
    switch (state){
        case 0:
            displayTourchOn();
            break;
        
        case 1:
            displayTourchOff();
            break;
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
    _displayOn = false;
    _display->ssd1306_command(SSD1306_DISPLAYOFF); 
    clearScreen();  

}

void Ui::displayOn(){
    enableSleepForDisplay();
    _displayOn = true;
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
    if(_allowDisplaySleep){
        _displaySleepCounter++;
        if(getDisplaySleepTime() == _displaySleepCounter) {
            resetDisplaySleepTimer();  
            disableSleepForDisplay();
        }
    }
}

void Ui::enableSleepForDisplay(void){
    _allowDisplaySleep = true;
    _displayTimeOut = false;
    resetDisplaySleepTimer();
}

void Ui::disableSleepForDisplay(void){
    _allowDisplaySleep = false;
    _displayTimeOut = true;
}

bool Ui::isDisplayTimeOut(void){
    return _displayTimeOut;
}

bool Ui::isDisplayOn(void){
    return _displayOn;
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
}

void Ui::printStringAt(int x, int y, String message){
    _display->setTextColor(WHITE,BLACK);
    _display->setCursor(x, y);
    _display->print(message); 
}

void Ui::printStringAt(int x, int y, String message, bool update){
    _display->setTextColor(WHITE,BLACK);
    _display->setCursor(x, y);
    _display->print(message); 
    if(update)
        _display->display();     
}

void Ui::printStringAt(int x, int y, int textSize, String message){
    _display->setTextSize(textSize);
    _display->setTextColor(WHITE,BLACK);
    _display->setCursor(x, y);
    _display->print(message);    
}

void Ui::printNumberAt(int x, int y, int size, int num){
    char buff[2];
    sprintf(buff, "%02d", num);
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(buff);         
}

void Ui::printNumberAt(int x, int y, int size, int num, int lenght){
    char buff[lenght];
    if(lenght == 2)
        sprintf(buff, "%02d", num);
    else
        sprintf(buff, "%03d", num);    
    _display->setTextSize(size);
    _display->setCursor(x, y);
    _display->print(buff);             
}

void Ui::printAppBar(int x, int y, String title){
    _display->fillRect(0,0,126,15,WHITE);
    _display->setCursor(x, y);
    _display->setTextSize(1);
    _display->setTextColor(BLACK);
    _display->print(title);
    _display->setTextColor(WHITE);
}

void Ui::showBatteryPercentage(char value){
    _display->fillRect(100, 0, 27, 15, BLACK);
    _display->drawRect(107, 3, 20, 10, WHITE);
    _display->fillRect(104, 5, 3, 5, WHITE);
    for (size_t i = 0; i < value; i++)
    {
        _display->drawLine(126 -i, 4, 126 -i, 11, WHITE);
    }    
}

void Ui::showTemprature(int x, int y, int size, char value){
    char buff[3];
    _display->setCursor(x, y);
    _display->setTextSize(size);
    sprintf(buff, "%02dc", value);
    _display->print(buff);
}

void Ui::enableDefaultFont(){
    _display->setFont();
    _display->setTextSize(1);
}

void Ui::setContrast(uint8_t value){
    _display->ssd1306_command(SSD1306_SETCONTRAST);
    _display->ssd1306_command(value);
}