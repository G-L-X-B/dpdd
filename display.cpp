#include "display.h"
#include "hardware.h"

void Display::drawDigits(int digits[]) {
  myOLED.printNumI(digits[0], LEFT_MARGIN, UPPER_MARGIN);
  myOLED.printNumI(digits[1], LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN), UPPER_MARGIN);
  myOLED.print(".", LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 2, UPPER_MARGIN - 5);
  myOLED.print(".", LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 2, UPPER_MARGIN - 15);
  myOLED.printNumI(digits[2], LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 3, UPPER_MARGIN);
  myOLED.printNumI(digits[3], LEFT_MARGIN + (FONT_XSIZE + FONT_INBETWEEN) * 4, UPPER_MARGIN);
}

void Display::drawSelectionArrows(int digit_focus) {
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

void Display::drawAlarmIndicator() {
  myOLED.drawCircle(127 - (CIRCLE_RADIUS + CIRCLE_MARGIN), CIRCLE_RADIUS + CIRCLE_MARGIN, CIRCLE_RADIUS);
}

#define MODE_CIRCLE_RADIUS 3
#define MODE_CIRCLE_MARGIN 3
void Display::drawModeIndicator(int current_mode, int num_of_modes) {
  int x = MODE_CIRCLE_RADIUS + MODE_CIRCLE_MARGIN;
  int first_y = 31 - (MODE_CIRCLE_RADIUS*2*num_of_modes + MODE_CIRCLE_MARGIN*(num_of_modes - 1)) / 2;
  int between = MODE_CIRCLE_RADIUS*2 + MODE_CIRCLE_MARGIN;
  for(int i = 0; i < num_of_modes; i++) {
    myOLED.drawCircle(x, first_y + i * between, MODE_CIRCLE_RADIUS); 
  }
  myOLED.setPixel(x, first_y + current_mode * between);
}

void Display::updateDisplay(OS* os) {
  myOLED.setFont(BigNumbers);
  
  int current_digits[4];
  os->getCurrentDigits(current_digits);
  int mode = os->getCurrentMode();
  bool focused = os->isFocused();
  int digit_focus = os->getCurrentDigitFocus();
  bool alarm_active = os->isAlarmActive();

  myOLED.clrScr();
  drawDigits(current_digits);
  drawModeIndicator(mode, 3);
  if(focused) {
    drawSelectionArrows(digit_focus);
  }
  if(alarm_active) {
    drawAlarmIndicator();
  }
  myOLED.update();
}

void Display::writeCurrLevel(int curr_level, int max_level) {
  myOLED.setFont(SmallFont);
  myOLED.clrScr();
  myOLED.print("Level: ", 63 - 5 * TEXT_FONT_XSIZE, 31 - TEXT_FONT_YSIZE / 2);
  myOLED.printNumI(curr_level, 63 + 2 * TEXT_FONT_XSIZE,31 - TEXT_FONT_YSIZE / 2);
  myOLED.print("/", 63 + 3 * TEXT_FONT_XSIZE,31 - TEXT_FONT_YSIZE / 2);
  myOLED.printNumI(max_level, 63 + 4 * TEXT_FONT_XSIZE,31 - TEXT_FONT_YSIZE / 2);
  myOLED.update();
}

void Display::failLevelMessage() {
  myOLED.setFont(SmallFont);
  myOLED.clrScr();
  myOLED.print("Level failed.", 63 - 13 * TEXT_FONT_XSIZE / 2, 31 - TEXT_FONT_YSIZE / 2);
  myOLED.update();  
}

void Display::successLevelMessage() {
  myOLED.setFont(SmallFont);
  myOLED.clrScr();
  myOLED.print("Level success!", 63 - 7 * TEXT_FONT_XSIZE, 31 - TEXT_FONT_YSIZE / 2);
  myOLED.update();  
}

void Display::timeoutMessage() {
  myOLED.setFont(SmallFont);
  myOLED.clrScr();
  myOLED.print("Game lost.", 63 - 5 * TEXT_FONT_XSIZE, 31 - TEXT_FONT_YSIZE / 2);
  myOLED.update();
}

void Display::finishMessage() {
  myOLED.setFont(SmallFont);
  myOLED.clrScr();
  myOLED.print("Congratulations!", 63 - 8 * TEXT_FONT_XSIZE, 31 - TEXT_FONT_YSIZE / 2);
  myOLED.update();
}
