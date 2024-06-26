#ifndef OS_H
#define OS_H

#include "hardware.h"
#include "os_sound_engine.h"

enum Mode { 
  kClock, 
  kSetAlarm, 
  kSetTime 
};

class OS {
  private:
    OSSoundEngine sound;
    int clock_hours;
    int clock_minutes;

    int alarm_hours;
    int alarm_minutes;
    bool alarm_active;
    bool alarm_ringing;
    
    int og_alarm_hours;
    int og_alarm_minutes;
    bool alarm_delayed;

    Mode mode;
    boolean mode_focused;
    #define NUM_OF_MODES 3

    boolean advanceTime();
    
    bool lit_buttons[5] = { false, false, false, false, false };

    bool processButton(Button button);

    int digits[4] = {0,0,0,0};
    int digit_limits[4] = {2,9,5,9};
    int digit_focus;
    bool processButtonFocused(Button button);

    bool processButtonAlarm(Button button);

    void initFocus();

    void setAlarm(int hours, int minutes);

    //hours must be in 0-23 range, minutes must be in 0-59 range
    void setTime(int hours, int minutes);
    
    void setOffAlarm();

  public:
    OS();
    
    void loop();

    void getCurrentDigits(int current_digits[]);
    
    bool isFocused();

    int getCurrentDigitFocus();

    bool isAlarmActive();

    int getCurrentMode();    
};
#endif 
