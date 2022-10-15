#include"Alarm.h"

Alarm::Alarm(void){}

Alarm::Alarm(void (*on)(), void (*off)(), int buzzerPin):
 _handlerOn(on), _handlerOff(off)
{
    //EasyBuzzer.setPin(buzzerPin);
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

void Alarm::alarmUpdate(void){
    EasyBuzzer.update();
}

void Alarm::playAlarm(){
    _alarmIsRunning = true;

    unsigned int frequency = 1000;  
    unsigned int onDuration = 50;
    unsigned int offDuration = 100;
    unsigned int beeps = 2;
    unsigned int pauseDuration = 500;
    unsigned int cycles = 10;

    EasyBuzzer.beep(
		frequency,		// Frequency in hertz(HZ). 
		onDuration, 	// On Duration in milliseconds(ms).
		offDuration, 	// Off Duration in milliseconds(ms).
		beeps, 			// The number of beeps per cycle.
		pauseDuration, 	// Pause duration.
		cycles 		// The number of cycle.
	);
}

void Alarm::stopAlarm(){
    _alarmIsRunning = false;
    EasyBuzzer.stopBeep();
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
