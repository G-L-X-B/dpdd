#ifndef DISPLAY_H
#define DISPLAY_H

#include "os.h"

#define LEFT_MARGIN 20
#define UPPER_MARGIN 19
#define FONT_XSIZE 14
#define FONT_YSIZE 24
#define FONT_INBETWEEN 4

#define TEXT_FONT_XSIZE 6
#define TEXT_FONT_YSIZE 8

#define ARROW_HALFSIZE 5
#define ARROW_MARGIN 15

#define CIRCLE_RADIUS 3
#define CIRCLE_MARGIN 3

class Display {
  private:
    static void drawDigits(int digits[]);

    static void drawSelectionArrows(int digit_focus);

    static void drawAlarmIndicator();

    static void drawModeIndicator(int current_mode, int num_of_modes);
    
  public:  
    static void updateDisplay(OS* os);

    static void writeCurrLevel(int curr_level, int max_level);

    static void failLevelMessage();
    
    static void successLevelMessage();
    
    static void timeoutMessage();
    
    static void finishMessage();
};

#endif
