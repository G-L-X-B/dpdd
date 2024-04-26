#ifndef HARDWARE_H
#define HARDWARE_H

#include <OLED_I2C.h>

#define LED_OFFSET 2

#define NO_BTN 0
#define BTN_UP_LED 2
#define BTN_DOWN_LED 3
#define BTN_LEFT_LED 4
#define BTN_RIGHT_LED 5
#define BTN_CONTROL_LED 6

#define BTN_OFFSET 7

#define NO_LED 0
#define BTN_UP 7
#define BTN_DOWN 8
#define BTN_LEFT 9
#define BTN_RIGHT 10
#define BTN_CONTROL 11

#define SPEAKER 12
#define OLED_P 13


// Arduino Uno/2009:
// ----------------------
// Display:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//           SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
extern OLED myOLED;
extern uint8_t BigNumbers[];

struct Button {
  byte button;
  byte led;
  int pitch;
};

void init_hardware();

extern boolean last_signals[5];

boolean debounce(byte button);

Button getCurrentButton();

#endif
