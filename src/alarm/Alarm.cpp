#include"Alarm.h"

Alarm::Alarm(void){}

Alarm::Alarm(void (*on)(), void (*off)(), int buzzerPin):
 _handlerOn(on), _handlerOff(off)
{
    this->_buzzerPin = buzzerPin;
    pinMode(buzzerPin, OUTPUT);

    analogWriteFreq(PWM_FREQ);
    analogWrite(_buzzerPin, 0);
}

void Alarm::turnAlarm(bool state){
    _alarmState = state;
}

bool Alarm::isAlarmOn(){
    return _alarmState;
}

void Alarm::setWhenAlarmOn(uint8 hour, uint8 minute, uint8 second){
    _hourOn = hour;
    _minuteOn = minute;
    _secondOn = second;
}

void Alarm::setWhenAlarmOff(uint8 hour, uint8 minute, uint8 second){
    _hourOff = hour;
    _minuteOff = minute;
    _secondOff = second;    
}

void Alarm::tick(uint8 hour, uint8 minute, uint8 second){
    if ((hour == this->_hourOn) && (minute == this->_minuteOn) && (second == this->_secondOn)) {
        if(_alarmEvent){
            (*_handlerOn)();
        }
    }

    if ((hour == this->_hourOff) && (minute == this->_minuteOff) && (second == this->_secondOff)) {
        (*_handlerOff)();
    }
}

void Alarm::playAlarm(){

    analogWrite(_buzzerPin, 100);
    _alarmIsRunning = true;
}

void Alarm::stopAlarm(){
    analogWrite(_buzzerPin, 0);
    _alarmIsRunning = false;
}

void Alarm::alarmUpdate(void){
    
}

bool Alarm::isAlarmRunning(void){
    return _alarmIsRunning;
}

int Alarm::getHourAlarm(){
    return _hourOn;
}

int Alarm::getMinuteAlarm(){
    return _minuteOn;
}

void Alarm::enableAlarmEvent(void){
    _alarmEvent = true;
}

void Alarm::disableAlarmEvent(void){
    _alarmEvent = false;
}
