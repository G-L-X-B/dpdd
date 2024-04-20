#ifndef OS_H
#define OS_H

#include "hardware.h"

class OS {
  private:
    int start_time;
    int clock_hours;
    int clock_minutes;
    int minutes_passed;

    int alarm_hours;
    int alarm_minutes;
    bool alarm_active;

    enum Mode { 
      kClock, 
      kSetAlarm, 
      kSetTime 
    };
    Mode mode;
    boolean mode_focused;
    #define NUM_OF_MODES = 3;

    boolean advanceTime() {
      int curr_minute = millis() - start_time / 60000;
      if(curr_minute != minutes_passed) {
        minutes_passed = curr_minutes;
        clock_minutes = (clock_minutes + 1) % 60;
        if(clock_minutes == 0) {
          clock_hours = (clock_hours + 1) % 24;
        }
        return true;
      }
      return false;
    }    

    void processButton(Button button) {
      if(!mode_focused) {        
        switch(button.button){
          case BTN_RIGHT:
            mode = (mode + 1) % NUM_OF_MODES;
            break;
          case BTN_LEFT:
            if(mode == 0){
              mode = NUM_OF_MODES - 1;
            } else {
              mode--;
            }
            break;
          case BTN_CONTROL:
            if(mode != kClock) {              
              mode_focused = true;
              initFocus();
            }
            break;
          default:
            break;
        } else {
          processButtonFocused(button);
        }
      }
    }

    int digits[4] = {0,0,0,0};
    int digit_limits[4] = {2,9,5,9};
    int digit_focus;
    void processButtonFocused(Button button) {
      switch(button.button) {
        case BTN_RIGHT:
          digit_focus = (digit_focus+1) % 4;
          break;
          
        case BTN_LEFT:
          if(digit_focus == 0) {
            digit_focus = 3;
          } else {
            digit_focus--;
          }
          break;
          
        case BTN_UP:
          if(digit_focus == 1 && digit[0] == 2) {
            digit[digit_focus] = (digit[digit_focus] + 1) % 4;
          } else {
            digit[digit_focus] = (digit[digit_focus] + 1) % (digit_limits[digit_focus] + 1);
          }
          break;
          
        case BTN_DOWN:
          if(digit_focus == 1 && digit[0] == 2 && digit[1] == 0) {
            digit[1] = 3
          } else if(digit[digit_focus] == 0) {
            digit[digit_focus] = digit_limits[digit_focus];
            if(digit_focus == 0 && digit[1] > 3) {
              digit[1] = 3;
            }
          } else {
            digit[digit_focus]--;
          }
          break;
          
        case BTN_CONTROL:
          hours = digit[0]*10 + digit[1];
          minutes = digit[2]*10 + digit[3];
          if(mode == kSetAlarm) {
            SetAlarm(hours, minutes);
          } else if (mode == kSetTime) {
            SetTime(hours, minutes);
          }
          mode_focused = false;
          break;
        default:
          break;
      }
    }

    void initFocus() {
      switch(mode) {
        case kSetTime:
          digits[0] = clock_hours / 10;
          digits[1] = clock_hours % 10;
          digits[2] = clock_minutes / 10;
          digits[3] = clock_minutes % 10;
          break;
        default:
          break;
      }
      digit_focus = 0;
    }

    void SetAlarm(int hours, int minutes) {
      if(alarm_hours == alarm_hours && minutes == alarm_minutes) {
        alarm_active != alarm_active;
        return;
      }
      alarm_hours = hours;
      alarm_minutes = minutes;
      alarm_active = true;
    }   

    //hours must be in 0-23 range, minutes must be in 0-59 range
    void setTime(int hours, int minutes) {
      clock_hours = hours;
      clock_minutes = minutes;
      start_time = millis();
      minutes_passed = 0;
    }

  public:
    OS() {
      start_time = millis();
      clock_hours = 0;
      clock_minutes = 0;      
      minutes_passed = 0;
      mode_focused = false;
    }
    
    void loop() {
      bool time_changed = advanceTime();
      Button curr_button = getCurrentButton();
      if(curr_button.button != NO_BTN) {
        processButton(curr_button);
      }
      if(time_changed || curr_button.button != NO_BTN) {
        //update display
      }
      if(time_changed && alarm_active && clock_hours == alarm_hours && clock_minutes == alarm_minutes) {
        //set off alarm
      }
    }
}
#endif 
