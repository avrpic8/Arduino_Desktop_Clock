/// Lcd pins
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/// Esp pins
#define ANALOG_PIN A0

#define PIN_ROTARY_IN1 D3
#define PIN_ROTARY_IN2 D6
#define PIN_SW  D7

//#define ONEWIRE_BUS D5
#define BUZZER D5

#define TOURCH_PIN D8

/// ntp server
#define NTP_SERVER "pool.ntp.org"


/// onewire
#define TEMP_SAMPLE_TIME 2


/// Pwm
#define PWM_FREQ 2000


/// alarm state
#define ON true
#define OFF false

/// menu names
#define Home 0
#define FM_RADIO 1
#define TIMER 2
#define TORCH 3
#define CLOCK_SETTING 4
#define WIFI_SETTING 5
#define DISPLAY 6
#define SOUND 7
#define EXIT 8

#define CLICKED 1
#define LONG_CLICKED 2


/// Radio freq test
#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  10500            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_VOLUME   4               ///< The volume that will be set by this sketch is level 4.


/// timer start checker
#define START_CHECK  (minuteCounter == 0 && secCounter > 0) || (minuteCounter > 0 && secCounter == 0) || (minuteCounter > 0 && secCounter > 0)

/// sleep checker
#define SLEEP_CHECK  !ui.isDisplayOn() && alarmClock.isAlarmOn() && !alarmClock.isAlarmRunning()