#ifndef DISPLAY_H
#define DISPLAY_H

#include "hardware.h"
#include "os.h"

#define LEFT_MARGIN 20
#define UPPER_MARGIN 19
#define FONT_XSIZE 14
#define FONT_YSIZE 24
#define FONT_INBETWEEN 4

#define ARROW_HALFSIZE 5
#define ARROW_MARGIN 15

#define CIRCLE_RADIUS 3
#define CIRCLE_MARGIN 3

class OS;

class Display {
  private:
    void drawDigits(int digits[]) {
      myOLED.printNumI(digits[0], LEFT_MARGIN, UPPER_MARGIN);
      myOLED.printNumI(digits[1], LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN), UPPER_MARGIN);
      myOLED.print('.', LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 2, UPPER_MARGIN);
      myOLED.printNumI(digits[2], LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 3, UPPER_MARGIN);
      myOLED.printNumI(digits[3], LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 4, UPPER_MARGIN);
    }

    void drawSelectionArrows(int digit_focus) {
      int char_pos = digit_focus < 2 ? digit_focus : digit_focus + 1;
      int x = LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * char_pos + FONT_XSIZE / 2;
      int upper_y = UPPER_MARGIN - ARROW_MARGIN;
      int lower_y = UPPER_MARGIN + FONT_YSIZE + ARROW_MARGIN;

      
      //upper arrow
      myOLED.drawLine(x, upper_y, x - ARROW_HALFSIZE, upper_y + ARROW_HALFSIZE);      
      myOLED.drawLine(x + 1, upper_y, x + 1 + ARROW_HALFSIZE, upper_y + ARROW_HALFSIZE);

      //lower arrow
      myOLED.drawLine(x, lower_y, x - ARROW_HALFSIZE, lower_y - ARROW_HALFSIZE);      
      myOLED.drawLine(x + 1, lower_y, x + 1 + ARROW_HALFSIZE, lower_y - ARROW_HALFSIZE);
    }

    void drawAlarmIndicator() {
      myOLED.drawCircle(127 - (CIRCLE_RADIUS + CIRCLE_MARGIN), CIRCLE_RADIUS + CIRCLE_MARGIN, CIRCLE_RADIUS);
    }
    

  public:  
    void updateDisplay(OS* os) {
      int current_digits[4];
      os->getCurrentDigits(current_digits);
      bool focused = os->isFocused();
      int digit_focus = os->getCurrentDigitFocus();
      bool alarm_active = os->isAlarmActive();

      myOLED.clrScr();
      drawDigits(current_digits);
      if(focused) {
        drawSelectionArrows(digit_focus);
      }
      if(alarm_active) {
        drawAlarmIndicator();
      }
      myOLED.update();
    }
};

#endif
