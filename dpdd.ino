#include "hardware.h"
#include "os.h"

OS os;

void setup() {
  init_hardware();
}

void loop() {
  os.loop();
}
