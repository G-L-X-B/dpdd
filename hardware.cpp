#include "hardware.h"

OLED myOLED(SDA, SCL, OLED_P);

static DS3231 RTC;

boolean last_signals[5] = {LOW, LOW, LOW, LOW, LOW};

void init_hardware() {
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);

  pinMode(BTN_UP_LED, OUTPUT);
  pinMode(BTN_DOWN_LED, OUTPUT);
  pinMode(BTN_LEFT_LED, OUTPUT);
  pinMode(BTN_RIGHT_LED, OUTPUT);
  digitalWrite(BTN_UP_LED, LOW);
  digitalWrite(BTN_DOWN_LED, LOW);
  digitalWrite(BTN_LEFT_LED, LOW);
  digitalWrite(BTN_RIGHT_LED, LOW);
  
  pinMode(SPEAKER, OUTPUT);

  RTC.begin();
  RTC.setHourMode(CLOCK_H24);

  myOLED.begin();
  myOLED.setFont(BigNumbers);
}

boolean debounce(byte button) {
  boolean current_signal = digitalRead(button);
  if(last_signals[button - BTN_OFFSET] != current_signal) {
    delay(5);
    current_signal = digitalRead(button);
    last_signals[button - BTN_OFFSET] = current_signal;
  }
  return current_signal;
}

Button getCurrentButton() {
  for(byte i = 0; i < 5; i++) {
    boolean ls = last_signals[i];
    boolean db = debounce(i + BTN_OFFSET);
    if(db && !ls) {
      return {i + BTN_OFFSET, i + LED_OFFSET, 300}; 
    }
  }
  return {NO_BTN, NO_LED, 0};
}
