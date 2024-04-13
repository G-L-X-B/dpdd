#ifndef GAME_H
#define GAME_H

#include "hardware.h"

#define MAX_SEQ 6
#define LIT_DURATION 500
#define MID_LED_DELAY 200
#define LOOP_DELAY 1000

#define INPUT_WAIT 60000

enum GameFSM {
  kStart,
  kDemo,
  kInput,
  kSuccess,
  kFail,
  kFinish
};

enum DemoFSM {
  kLEDDown,
  kLEDUp,
  kReloop
};

struct {
public:
  void play() {
    setupGame();
    game();
  }

private:
  void setupGame() {
    state = kStart;
    lit = 0;
    lvl = 1;
    startTime = millis();
    pending = {0, 0};
    generate();
  }

  void game() {
    unsigned int elapsed;
    while (true) {
      elapsed = millis() - startTime;
      pending = getCurrentButton();
      switch (state) {
        case kStart:
          if (++lvl > 6) {
            state = kFinish;
            demo_state = kLEDDown;
          } else {
            state = kDemo;
          }
          break;
        case kDemo:
          if (pending.button == 0) {
            demo(elapsed);
            break;
          } else {
            endDemo();
            state = kInput;
          }
        case kInput:
          input(elapsed);
          break;
        case kSuccess:
          lit = 0;
          break;
        case kFail:
          break;
        case kFinish:
          break;
      }
    }
  }

  void generate() {
    for (byte i = 0; i < MAX_SEQ; ++i) {
      sequence[i] = rand() % 4;
    }
  }

  void demo(unsigned int elapsed) {
    switch (demo_state) {
      case kLEDDown:
        if (elapsed >= MID_LED_DELAY) {
          digitalWrite(BTN_LED_UP + sequence[lit], HIGH);
          demo_state = kLEDUp;
        }
        break;
      case kLEDUp:
        if (elapsed >= LIT_DURATION) {
          digitalWrite(BTN_LED_UP + sequence[lit], LOW);
          if (++lit >= lvl) {
            demo_state = kReloop;
            lit = 0;
          }
        }
        break;
      case kReloop:
        if (elapsed >= LOOP_DELAY)
          demo_state = kLEDDown;
        break;
    }
  }

  void endDemo() {
    for (int i = 0; i < 4; ++i)
      digitalWrite(BTN_LED_UP + i, LOW);
    lit = 0;
  }

  void input(unsigned int elapsed) {
    if (elapsed >= INPUT_WAIT) {
      state = kSuccess;
      return;
    }
    int in = pending.button - BTN_UP;
    if (sequence[lit] == in) {
      if (++lit == lvl) {
        state = kSuccess;
      }
    } else {
      state = kFail;
    }
  }

  byte sequence[MAX_SEQ] = {0};
  Button pending;
  unsigned int startTime;
  GameFSM state;
  DemoFSM demo_state;
  byte lit;
  byte lvl;
} Game;

#endif
