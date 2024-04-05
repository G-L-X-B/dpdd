//#include "OLED_I2C.h"
#include "game.h"

#include "hardware.h"

struct Button {
  const byte button;
  const byte led;
};

void setup() {
  init_hardware();
}

void loop() {
}
