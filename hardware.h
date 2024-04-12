#ifndef HARDWARE_H
#define HARDWARE_H

#define BTN_UP 10
#define BTN_DOWN 11
#define BTN_LEFT 12
#define BTN_RIGHT 13

#define BTN_UP_LED 1
#define BTN_DOWN_LED 2
#define BTN_LEFT_LED 3
#define BTN_RIGHT_LED 4

struct Button {
  const byte button;
  const byte led;
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

Button getCurrentButton() {
  for(int i = 0; i < 4; i++) {
    if(digitalRead(i + 2)){
      if(debounce(i + 2)){
        return {i + 2, i + 6};        
      }
    }
    return {0, 0};
  }
}

boolean debounce(byte button) {
  boolean current = digitalRead(button);
  if(!current) {
    delay(5);
    current = digitalRead(button);
    return current;
  }
}

#endif
