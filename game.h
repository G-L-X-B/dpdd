#ifndef GAME_H
#define GAME_H

#include "dislpay.h"
#include "hardware.h"
#include "os_sound_engine.h"

#define MAX_SEQ 6
#define LIT_DURATION 500
#define MID_LED_DELAY 200
#define LOOP_DELAY 1000
#define INTRO_DELAY 2000

#define LINE_DELAY 3000

#define INPUT_WAIT 60000

#define INTRO_LINE_NUM 8

enum GameFSM {
  kIntro,    // The very beginning. Tell the rules, immediately start playing on user input.
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
    state = kIntro;
    demo_state = kLEDDown;
    press_lit = NO_LED;
    line = 0;
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
      processPressedButton(pending);
      switch (state) {
        case kIntro:
          playIntro(elapsed);
          break;
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
          Display::writeLine(success_line);
          delay(LINE_DELAY);
          state = kStart;
          break;
        case kFail:
          Display::writeLine(fail_line);
          delay(LINE_DELAY);
          return false;
        case kFinish:
          Display::writeLine(finish_line);
          delay(LINE_DELAY);
          return true;
      }
      sound_engine->loop();
    }
  }

  inline void processPressedButton(Button button) {
      if (pending.led != NO_LED) {
        press_lit = pending.led;
        digitalWrite(press_lit, HIGH);
        sound_engine->button_sound_persistent(button);
      } else if (press_lit != NO_LED) {
        digitalWrite(press_lit, LOW);
        press_lit = NO_LED;
      }
  }

  void generate() {
    for (byte i = 0; i < MAX_SEQ; ++i) {
      sequence[i] = rand() % 4;
    }
  }

  void playIntro(unsigned int elapsed) {
    if (elapsed < INTRO_DELAY) return;

    if (line >= INTRO_LINE_NUM) {
      state = kStart;
    } else {
      Display::writeLine(intro_lines[line++];
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
    demo_state = kLEDDown;
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

  String intro_lines[INTRO_LINE_NUM] = {
    "Henlo!",
    "To disable the alarm",
    "You have to beat",
    "The Game",
    "Watch the order",
    "in which buttons are lit",
    "and repeat it.",
    "GL HF;)"
  }

  String success_line = "Ok, what's next!?";
  String fail_line = "OMG u suck";
  String finish_line = "Are you good or am I bad?";
  OSSoundEngine *sound_engine;
  byte sequence[MAX_SEQ] = {0};
  Button pending;
  unsigned int startTime;
  GameFSM state;
  DemoFSM demo_state;
  int press_lit;
  byte lit;
  byte lvl;
  byte line;
} Game;

#endif
