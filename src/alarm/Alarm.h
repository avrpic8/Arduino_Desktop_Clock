#ifndef Alarm_h
#define Alarm_h

#include<Arduino.h>   /// if not exist error occurd
#include"../definition.h"

class Alarm
{
public:
    
    Alarm(void);
    Alarm(void (*On)(), void (*Off)(), int buzzerPin);
    void turnAlarm(bool state);
    bool isAlarmOn(void);
    void setWhenAlarmOn(uint8 hour, uint8 minute, uint8 second);
    void setWhenAlarmOff(uint8 hour, uint8 minute, uint8 second);
    void tick(uint8 hour, uint8 minute, uint8 second);
    void alarmUpdate(void);
    void playAlarm();
    void stopAlarm();

    int getHourAlarm(void);
    int getMinuteAlarm(void);
    bool isAlarmRunning(void);

    void enableAlarmEvent(void);
    void disableAlarmEvent(void);

    void toggleBuzzer(bool state);

private:
    uint8_t _hourOn = 0;        //!< Alarm hour
    uint8_t _minuteOn = 0;      //!< Alarm minute
    uint8_t _secondOn = 15;      //!< Alarm second

    uint8_t _hourOff;        //!< Alarm hour
    uint8_t _minuteOff;      //!< Alarm minute
    uint8_t _secondOff;      //!< Alarm second

    bool _alarmState = true;
    bool _alarmIsRunning;
    bool _alarmEvent = true;

    void (*_handlerOn)(); //!< Alarm on handler
    void (*_handlerOff)(); //!< Alarm off handler

    uint8 _buzzerPin;
};

#endif

