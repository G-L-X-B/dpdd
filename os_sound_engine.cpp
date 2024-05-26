#include "os_sound_engine.h"
#include "Arduino.h"
#include "hardware.h"

#define MID_SIGNAL 100
#define SIGNAL 100
#define NO_OF_SIGNALS 3
#define END_OF_CYCLE 500

#define BUTTON_SOUND_DURATION 100


void OSSoundEngine::alarm_loop() {
  int current_time = millis() - start_time;
  if(current_signal > NO_OF_SIGNALS) {
    if(current_time > NO_OF_SIGNALS * SIGNAL + (NO_OF_SIGNALS - 1) * MID_SIGNAL + END_OF_CYCLE) {
      start_time = millis();
      current_signal = 1;      
    }
    return;
  }
  if((current_time > (current_signal - 1) * SIGNAL + (current_signal - 1) * MID_SIGNAL) && (current_time <= (current_signal) * SIGNAL + (current_signal - 1) * MID_SIGNAL)) {
    tone(SPEAKER, NOTE_C5);
  }
  if(current_time > (current_signal) * SIGNAL + (current_signal - 1) * MID_SIGNAL) {
    ++current_signal;
    noTone(SPEAKER);
  }
}

OSSoundEngine::OSSoundEngine() {
  alarm_ringing = false;
  currently_pressed = 0;
  highest_priority_button = NO_BTN;
}

void OSSoundEngine::start_alarm() {
  start_time = millis();
  current_signal = 1;
  alarm_ringing = true;
}

void OSSoundEngine::stop_alarm() {
  alarm_ringing = false;
  noTone(SPEAKER);  
}

//int button_pitches[5] = {NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4};
void OSSoundEngine::button_sound(Button button) {
  if(!alarm_ringing) {
    tone(SPEAKER, button_pitches[button.button - BTN_OFFSET], BUTTON_SOUND_DURATION);    
  }
}

void OSSoundEngine::button_sound_persistent(Button button) {
  if(!alarm_ringing && button.button != NO_BTN) {
    tone(SPEAKER, button_pitches[button.button - BTN_OFFSET]);
    ++currently_pressed;
    button_priorities[button.button - BTN_OFFSET] = currently_pressed;
    highest_priority_button = button.button;
  }
}

void OSSoundEngine::button_sound_persistent_loop() {
  //updating button queue
  for(int i = 0; i < 5; i++) {
    if(button_priorities[i] != 0 && !last_signals[i]) {
      --currently_pressed;
      for(int j = 0; j < 5; j++) {
        if(button_priorities[j] > button_priorities[i]) {
          --button_priorities[j];
        }
      }
      button_priorities[i] = 0;
    }
  }
  
  if(highest_priority_button != NO_BTN && button_priorities[highest_priority_button - BTN_OFFSET] == 0) {
    //updating tone
    byte max_priority = 0;
    highest_priority_button = NO_BTN;
    for(int i = 0; i < 5; i++) {
      if(button_priorities[i] > max_priority) {
        max_priority = button_priorities[i];
        highest_priority_button = i + BTN_OFFSET;
      }
    }
    if(highest_priority_button == NO_BTN) {
      noTone(SPEAKER); 
    } else {
      tone(SPEAKER, button_pitches[highest_priority_button - BTN_OFFSET]);
    }
  }
}

void OSSoundEngine::loop() {
  button_sound_persistent_loop();
  if(alarm_ringing) {
    alarm_loop();        
  }
}
