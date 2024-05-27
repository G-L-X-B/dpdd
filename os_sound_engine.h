#ifndef OS_SOUND_ENGINE_H
#define OS_SOUND_ENGINE_H

#include "hardware.h"
#include "pitches.h"


class OSSoundEngine {
  private:
    long start_time;
    int current_signal;
    bool alarm_ringing;
    int button_pitches[5] = {NOTE_A4, NOTE_GS5, NOTE_D5, NOTE_E5, NOTE_E4};

    byte currently_pressed;
    byte highest_priority_button;
    byte button_priorities[5] = {0,0,0,0,0};

    void alarm_loop();
    void button_sound_persistent_loop();
  public:
    OSSoundEngine();
  
    void start_alarm();
    
    void stop_alarm();

    void button_sound(Button button);
    
    void button_sound(int button, int duration);

    void button_sound_persistent(Button button);

    void loop();
};


#endif
