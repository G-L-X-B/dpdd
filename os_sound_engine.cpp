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

void OSSoundEngine::loop() {
  if(alarm_ringing) {
    alarm_loop();        
  }
}
