#ifndef OS_SOUND_ENGINE_H
#define OS_SOUND_ENGINE_H

#include "pitches.h"
#include "hardware.h"


class OSSoundEngine {
  private:
    long start_time;
    int current_signal;
    bool alarm_ringing;

    void alarm_loop();
    
  public:
    OSSoundEngine();
  
    void start_alarm();
    
    void stop_alarm();

    void button_sound(Button button);

    void loop();
};


#endif
