#ifndef easybutton_h
#define easybutton_h

#include <Arduino.h>
#include <ezButton.h>
#include "debug.h"

const static int BUTTON_NOT_PRESSED  = 0;
const static int BUTTON_SHORT_PRESS  = 1;
const static int BUTTON_HOLD_PRESS   = 2;
const static int BUTTON_LONG_PRESS   = 3;

/**
 * Use EasyButton to track state on a specific pin.
 * It uses the onboard resistor (INPUT_PULL).
 * Supports tracking following conditions:
 *   BUTTON_NOT_PRESSED - no state change
 *   BUTTON_SHORT_PRESS - Short Press on release
 *   BUTTON_LONG_PRESS - Long Press on release
 *   BUTTON_HOLD_PRESS - Press and Hold
 * eg. EasyButton selectorOnPin8(8);
 *
 * Call the begin() function in the setup to initilize the pin mode, and set a debouce value.
 * eg. selectorOnPin8.begin(25);
 * 
 * Check the state, and use the defines to determine current state:
 * eg.int selectedMode = selectorOnPin8.checkState()
 * 
 * REQUIRED LIBRARIES - ezButton
 */
class EasyButton
{
  private:
    ezButton button;
    unsigned long pressedTime  = 0;
    unsigned long releasedTime = 0;
    bool isPressing = false;
    bool isLongDetected = false;
    const int SHORT_PRESS_TIME = 200;  // 200 milliseconds
    const int LONG_PRESS_TIME  = 1000; // 1000 milliseconds

  public:
    EasyButton(uint8_t pin) : button(pin) {
      button.setDebounceTime(50); // set debounce time to 50 milliseconds
    }

    void begin(int debounce) {
      button.setDebounceTime(debounce);
    }

    int checkState() {
      button.loop(); // MUST call the loop() function first
    
      if(button.isPressed()){
        debug.log("button pressed");
        pressedTime = millis();
        isPressing = true;
        isLongDetected = false;
      }
    
      if(button.isReleased() && isPressing == true) {
        debug.log("button released");
        releasedTime = millis();
        long pressDuration = releasedTime - pressedTime;

        // check if we have a short press
        if( pressDuration <= LONG_PRESS_TIME ) {
          isPressing = false;
          isLongDetected = false;
          debug.log("A short press is detected");
          return BUTTON_SHORT_PRESS;
        }
        if( pressDuration >= LONG_PRESS_TIME ) {
          isPressing = false;
          isLongDetected = true;
          debug.log("A long press is detected");
          return BUTTON_LONG_PRESS;
        }
      }
/* TODO - might want this behaviour to be configurable
      if(isPressing == true && isLongDetected == false) {
        long pressDuration = millis() - pressedTime;
    
        if( pressDuration > LONG_PRESS_TIME ) {
          debug.log("A long press is detected");
          isLongDetected = true;
          return BUTTON_LONG_PRESS;
        }
      }
*/
      // held down
      if (isPressing) {
        long pressDuration = millis() - pressedTime;
        // looping is fast, so release may not be detected
        if (pressDuration > 500) {
          debug.log("Button press held");
          return BUTTON_HOLD_PRESS;
        }
      }
      
      // nothing happening
      return BUTTON_NOT_PRESSED;
    }
};

#endif
