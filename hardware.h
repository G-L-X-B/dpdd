#ifndef HARDWARE_H
#define HARDWARE_H

#define NO_BTN 0
#define BTN_UP 10
#define BTN_DOWN 11
#define BTN_LEFT 12
#define BTN_RIGHT 13

#define NO_LED 0
#define BTN_UP_LED 1
#define BTN_DOWN_LED 2
#define BTN_LEFT_LED 3
#define BTN_RIGHT_LED 4

struct Button {
  byte button;
  byte led;
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
}

boolean debounce(byte button) {
  boolean current = digitalRead(button);
  if(!current) {
    delay(5);
    current = digitalRead(button);
    return current;
  }
}

Button getCurrentButton() {
  for(byte i = 0; i < 4; i++) {
    if(digitalRead(i + 2)) {
      if(debounce(i + 2)) {
        return {i + 2, i + 6};        
      }
    }
    return {NO_BTN, NO_LED};
  }
}

#endif
