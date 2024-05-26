#ifndef GAME_H
#define GAME_H

#include "hardware.h"
#include "os_sound_engine.h"

#define MAX_SEQ 6
#define LIT_DURATION 500
#define MID_LED_DELAY 200
#define LOOP_DELAY 1000

#define INPUT_WAIT 60000

enum GameFSM {
  kStart,    // New level, increment level and proceed playing.
  kDemo,     // The player hasn't started to entering sequence yet.
             // Loop the demo and await input for INPUT_WAIT.
  kInput,    // The player enters the sequence. No demo, wait for INPUT_WAIT.
  kSuccess,  // The player passes to the next level.
  kFail,     // The player has failed the game.
  kFinish    // The game was completed, congratulations! Play a melody and quit.
};

enum DemoFSM {
  kLEDDown, // No LED is lit. Light the next one.
  kLEDUp,   // The current LED is lit. Set it down, proceed to the next one.
  kReloop   // Demo is over. Start again.
};

struct {
public:
  bool play() {
    setupGame();
    return game();
  }

  void setSoundEngine(OSSoundEngine *engine) {
    sound_engine = engine;
  }
private:
  void setupGame() {
    state = kStart;
    demo_state = kLEDDown;
    lit = 0;
    lvl = 0; // will be set to 1 one the first iteration
    startTime = millis();
    pending = {0, 0};
    generate();
  }

  // Loop until the game is completed or failed or timed out
  bool game() {
    unsigned int elapsed;
    while (true) {
      elapsed = millis() - startTime;
      pending = getCurrentButton();
      switch (state) {
        case kStart:
          if (++lvl > MAX_SEQ) {
            state = kFinish;
            demo_state = kLEDDown;
          } else {
            state = kDemo;
          }
          break;
        case kDemo:
          if (pending.button == NO_BTN) {
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
          return false;
        case kFinish:
          return true;
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
          digitalWrite(LED_OFFSET + sequence[lit], HIGH);
          demo_state = kLEDUp;
        }
        break;
      case kLEDUp:
        if (elapsed >= LIT_DURATION) {
          digitalWrite(LED_OFFSET + sequence[lit], LOW);
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
      state = kFail;
      return;
    }
    int in = pending.button - BTN_OFFSET;
    if (sequence[lit] == in) {
      if (++lit >= lvl) {
        state = kSuccess;
      }
    } else {
      state = kFail;
    }
  }

  OSSoundEngine *sound_engine;
  byte sequence[MAX_SEQ] = {0};
  Button pending;
  unsigned int startTime;
  GameFSM state;
  DemoFSM demo_state;
  byte lit;
  byte lvl;
} Game;

#endif
