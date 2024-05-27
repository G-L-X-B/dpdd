#ifndef GAME_H
#define GAME_H

#include "display.h"
#include "hardware.h"
#include "os_sound_engine.h"

#define MAX_SEQ 6
#define LIT_DURATION 500
#define MID_LED_DELAY 200
#define LOOP_DELAY 1000
#define INTRO_DELAY 2000

#define LINE_DELAY 800

#define INPUT_WAIT 60000

#define INTRO_LINE_NUM 8

enum GameFSM {
  kIntro,    // The very beginning. Tell the rules, immediately start playing on user input.
  kStart,    // New level, increment level and proceed playing.
  kDemo,     // The player hasn't started to entering sequence yet.
             // Loop the demo and await input for INPUT_WAIT.
  kInput,    // The player enters the sequence. No demo, wait for INPUT_WAIT.
  kPending,  // The player already entered sequence to fail or succeed, but the game waits for him to clear the input
  kSuccess,  // The player passes to the next level.
  kFail,     // The player has failed the sequence
  
  kTimeout,  // The player has failed the game due to not being active for a long time
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
    OSSoundEngine *sound_engine;
    byte sequence[MAX_SEQ] = {0};
    unsigned long startTime;
    unsigned long lastInputTime;
    GameFSM state;
    DemoFSM demo_state;
    
    byte lit;
    byte lvl;
    byte curr_input;
  
    bool pending_success;
    
    bool lit_buttons[5] = { false, false, false, false, false };
    
    void setupGame() {
      state = kIntro;
      demo_state = kLEDDown;
      lit = 0;
      lvl = 0; // will be set to 1 one the first iteration
      curr_input = 0;
      pending_success = false;
      startTime = millis();
      lastInputTime = millis();
      srand(millis());
      generate();
    }
  
    // Loop until the game is completed or failed or timed out
    bool game() {
      unsigned int elapsed;
      while (true) {
        elapsed = millis() - startTime;
        Button curr_button = getCurrentButton();

        for(int i = 0; i < 5; i++) {
          if(lit_buttons[i] && !last_signals[i]) {
            lit_buttons[i] = false;
            digitalWrite(i + LED_OFFSET, LOW);      
          }
        }
        sound_engine->loop();
        
        switch (state) {
          case kIntro:
            playIntro(elapsed);
            break;
            
          case kStart:
            if (++lvl > MAX_SEQ) {
              state = kFinish;
              demo_state = kLEDDown;
            } else {
              Display::writeCurrLevel(lvl, MAX_SEQ);
              state = kDemo;
            }
            break;
            
          case kDemo:
            if (curr_button.button == NO_BTN || curr_button.button == BTN_CONTROL) {
              demo(elapsed);
              break;
            } else {
              endDemo();
              state = kInput;
            }            
          case kInput:
            input(curr_button, elapsed);
            break;

          case kPending:
            pend();
            break;
            
          case kSuccess:
            Display::successLevelMessage();
            delay(LINE_DELAY);
            lit = 0;
            curr_input = 0;
            state = kStart;
            break;
            
          case kFail:
            Display::failLevelMessage();
            delay(LINE_DELAY);
            lit = 0;
            lvl = 0;
            curr_input = 0;
            state = kStart;
            generate();
            break;
            
          case kTimeout: 
            Display::timeoutMessage();
            delay(LINE_DELAY);           
            return false;
            
          case kFinish:
            Display::finishMessage();
            delay(LINE_DELAY);
            return true;
        }
      }
    }
  
    void generate() {
      for (byte i = 0; i < MAX_SEQ; ++i) {
        sequence[i] = rand() % 4;
      }
    }
  
    void playIntro(unsigned long elapsed) {
      //if (elapsed < INTRO_DELAY) return;
      state = kStart;
    }
  
    void demo(unsigned long elapsed) {
      if (millis() - lastInputTime >= INPUT_WAIT) {
        state = kTimeout;
        return;
      }
      switch (demo_state) {
        case kLEDDown:
          if (elapsed >= MID_LED_DELAY) {
            digitalWrite(LED_OFFSET + sequence[lit], HIGH);
            sound_engine->button_sound(BTN_OFFSET + sequence[lit], LIT_DURATION);
            demo_state = kLEDUp;
            startTime = millis();
          }
          break;
        case kLEDUp:
          if (elapsed >= LIT_DURATION) {
            digitalWrite(LED_OFFSET + sequence[lit], LOW);
            demo_state = kLEDDown;
            if (++lit >= lvl) {
              demo_state = kReloop;
              lit = 0;
            }
            startTime = millis();
          }
          break;
        case kReloop:
          if (elapsed >= LOOP_DELAY) {
            demo_state = kLEDDown;
            startTime = millis();            
          }
          break;
      }
    }
  
    void endDemo() {
      for (int i = 0; i < 4; ++i)
        digitalWrite(LED_OFFSET + i, LOW);
      lit = 0;
      demo_state = kLEDDown;
    }
  
    void input(Button button, unsigned long elapsed) {
      if (elapsed >= INPUT_WAIT) {
        state = kTimeout;
        return;
      }
      if (button.button == NO_BTN || button.button == BTN_CONTROL) {
        return;
      }
      
      sound_engine->button_sound_persistent(button);
      lit_buttons[button.button - BTN_OFFSET] = true;
      digitalWrite(button.led, HIGH);
      
      if (sequence[curr_input] == button.button - BTN_OFFSET) {
        if (++curr_input >= lvl) {
          state = kPending;
          pending_success = true;
        }
      } else {
        state = kPending;
        pending_success = false;
      }
      startTime = millis();
      lastInputTime = millis();
    }

    void pend() {
      if (millis() - lastInputTime >= INPUT_WAIT) {
        state = kTimeout;
        return;
      }
      for(int i = 0; i < 5; ++i) {
        if(last_signals[i]){
          return;
        }
      }
      if(pending_success) {
        state = kSuccess;
      } else {
        state = kFail;
      }
      startTime = millis();
    }
} Game;

#endif
