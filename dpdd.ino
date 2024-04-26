#include "hardware.h"
#include "display.h"
#include "game.h"
#include "os.h"

OS os;

void setup() {
  init_hardware();
  Display::updateDisplay(&os);
}

void loop() {
  os.loop();  
}
