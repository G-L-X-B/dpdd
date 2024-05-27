#include "hardware.h"
#include "display.h"
#include "game.h"
#include "os.h"

//OS os;
OSSoundEngine sound;

void setup() {
  init_hardware();
  Game.setSoundEngine(&sound);
  bool game_win = Game.play();
}

void loop() {
  //os.loop();  
}
