#ifndef HARDWARE_H
#define HARDWARE_H

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
#define OLED 13

struct Button {
  byte button;
  byte led;
  int pitch;
};

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
}

boolean last_signals[5] = {LOW, LOW, LOW, LOW, LOW}

boolean debounce(byte button) {
  boolean current_signal = digitalRead(button);
  if(last_signals[button - BTN_OFFSET] != current_signal) {
    delay(5);
    current = digitalRead(button);
    last_signals[button - BTN_OFFSET] = current;
    return current;
  }
}

Button getCurrentButton() {
  for(byte i = 0; i < 5; i++) {
    if(debounce(i + BTN_OFFSET) && (last_signals[i] == LOW)) {
      return {i + BTN_OFFSET, i + LED_OFFSET, 300}; 
    }
    return {NO_BTN, NO_LED, 0};
  }
}

#endif
