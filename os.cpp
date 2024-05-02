#include "os.h"
#include "display.h"

boolean OS::advanceTime() {
  if(clock_minutes != RTC.getMinutes() || clock_hours != RTC.getHours()) {
    clock_minutes = RTC.getMinutes();
    clock_hours = RTC.getHours();
    return true;
  }
  return false;
}

void OS::processButton(Button button) {
  if(alarm_ringing) {
    processButtonAlarm(button);
  } else if(mode_focused) {
    processButtonFocused(button);
  } else {
    switch(button.button) {
      case BTN_DOWN:
        mode = (mode + 1) % NUM_OF_MODES;
        break;
      case BTN_UP:
        if(mode == 0){
          mode = NUM_OF_MODES - 1;
        } else {
          mode = mode - 1;
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
    }
  }
}

void OS::processButtonFocused(Button button) {
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
      if(digit_focus == 1 && digits[0] == 2) {
        digits[digit_focus] = (digits[digit_focus] + 1) % 4;
      } else if (digit_focus == 0 && digits[0] == 1 && digits[1] > 3) {
        digits[0] = 2;
        digits[1] = 3;
      } else {
        digits[digit_focus] = (digits[digit_focus] + 1) % (digit_limits[digit_focus] + 1);
      }
      break;
      
    case BTN_DOWN:
      if(digit_focus == 1 && digits[0] == 2 && digits[1] == 0) {
        digits[1] = 3;
      } else if(digits[digit_focus] == 0) {
        digits[digit_focus] = digit_limits[digit_focus];
        if(digit_focus == 0 && digits[1] > 3) {
          digits[1] = 3;
        }
      } else {
        digits[digit_focus]--;
      }
      break;
      
    case BTN_CONTROL:
      int hours = digits[0]*10 + digits[1];
      int minutes = digits[2]*10 + digits[3];
      if(mode == kSetAlarm) {
        setAlarm(hours, minutes);
      } else if (mode == kSetTime) {
        setTime(hours, minutes);
      }
      mode_focused = false;
      break;
    default:
      break;
  }
}

void OS::initFocus() {
  switch(mode) {
    case kSetTime:
      digits[0] = clock_hours / 10;
      digits[1] = clock_hours % 10;
      digits[2] = clock_minutes / 10;
      digits[3] = clock_minutes % 10;
      break;
    case kSetAlarm:
      digits[0] = alarm_hours / 10;
      digits[1] = alarm_hours % 10;
      digits[2] = alarm_minutes / 10;
      digits[3] = alarm_minutes % 10;
    default:
      break;
  }
  digit_focus = 0;
}

void OS::processButtonAlarm(Button button) {
  if(!alarm_delayed) {
    og_alarm_hours = alarm_hours;
    og_alarm_minutes = alarm_minutes;
  }
  alarm_ringing = false;
  for(int i = 0; i < 5; i++) {
    digitalWrite(i + LED_OFFSET, LOW);
  }
  //todo: stop ringing
  bool game_win = true; //todo: call game

  if(game_win) {
    alarm_delayed = false;
    alarm_hours = og_alarm_hours;
    alarm_minutes = og_alarm_minutes;
  } else {
    alarm_delayed = true;
    alarm_minutes = clock_minutes + 5;
    alarm_hours = clock_hours;
    if(alarm_minutes > 59) {
      alarm_minutes = alarm_minutes % 60; 
      alarm_hours = (clock_hours + 1) % 24;
    }
  }
  alarm_active = true;
}

void OS::setAlarm(int hours, int minutes) {
  if(hours == alarm_hours && minutes == alarm_minutes) {
    alarm_active = !alarm_active;
    return;
  }
  alarm_hours = hours;
  alarm_minutes = minutes;
  alarm_active = true;
}

void OS::setTime(int hours, int minutes) {
  clock_minutes = minutes;
  clock_hours = hours;
  RTC.setTime(hours, minutes, 0);
}

void OS::setOffAlarm() {
  for(int i = 0; i < 5; i++) {
    digitalWrite(i + LED_OFFSET, HIGH);
  }
  //todo: start ringing
  alarm_ringing = true;  
  alarm_active = false;
}

OS::OS() {
  clock_hours = 0;
  clock_minutes = 0;

  alarm_hours = 0;
  alarm_minutes = 0;
  alarm_active = false;
  alarm_ringing = false;
  
  mode_focused = false;
}

void OS::loop() {
  bool time_changed = advanceTime();
  Button curr_button = getCurrentButton();
  if(curr_button.button != NO_BTN) {
    processButton(curr_button);
  }
  if(time_changed && alarm_active && clock_hours == alarm_hours && clock_minutes == alarm_minutes) {
    setOffAlarm();
  }
  if(time_changed || curr_button.button != NO_BTN) {
    Display::updateDisplay(this);
  }
}

void OS::getCurrentDigits(int current_digits[]) {
  if(mode_focused) {
    current_digits[0] = digits[0];
    current_digits[1] = digits[1];
    current_digits[2] = digits[2];
    current_digits[3] = digits[3];
  } else if (mode == kSetAlarm){
    current_digits[0] = alarm_hours / 10;
    current_digits[1] = alarm_hours % 10;
    current_digits[2] = alarm_minutes / 10;
    current_digits[3] = alarm_minutes % 10;
  } else {
    current_digits[0] = clock_hours / 10;
    current_digits[1] = clock_hours % 10;
    current_digits[2] = clock_minutes / 10;
    current_digits[3] = clock_minutes % 10;    
  }
}

bool OS::isFocused() {
  return mode_focused;
}

int OS::getCurrentDigitFocus() {
  if(mode_focused) {
    return digit_focus;
  } else {
    return -1;
  }
}

bool OS::isAlarmActive() {
  return alarm_active;
}

int OS::getCurrentMode() {
  return mode;
}
