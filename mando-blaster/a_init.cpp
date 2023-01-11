/*
 * name:  a_init.cpp
 * Author:  FriskyDingo
 * Date:  2021-07-14
 * Description: Code for Mando blaster
 */
#include <Arduino.h>
#include "config.h"
#include "easybutton.h"
#include "easycounter.h"
#include "easyaudio.h"
#include "easyledv3.h"

/**
 * All components are controlled or enabled by "config.h". Before running, 
 * review and change all configurations based on your setup.
 * 
 * There's no need to change any of the following code or functions. 
 */
EasyAudio audio(AUDIO_RX_PIN, AUDIO_TX_PIN);
EasyButton trigger(TRIGGER_PIN);

EasyLedv3<FIRE_LED_CNT, FIRE_LED_PIN> fireLed;
ezBlasterShot blasterShot(fireLed.RED, fireLed.ORANGE);  // initialize colors to starting fire mode

EasyCounter fireCounter;
EasyCounter stunCounter;


/**
 *   Variables for tracking trigger state
 *   Variables must be marked as volatile because they are updated in the ISR.
 */
uint8_t selectedTriggerMode = AMMO_MODE_FIRE;  // sets the fire mode to blaster to start
bool playStartupSound = 1;                     // play ower up sound on startup

/**
 * function declarations
 */
// main loop functions
void powerUp(void);
bool checkTriggerSwitch(void);
void handleLedDisplay(void);
void handleAmmoDown(void);
void setNextAmmoMode();
void changeAmmoMode(int mode);
void reloadAmmo(void);
// convenience functions
EasyCounter& getTriggerCounter();
uint8_t getSelectedTrack(uint8_t idx);

void setup() {
  Serial.begin(115200);
  DBGLN("Starting setup");

  // Initialize the ammo counters for different modes
  fireCounter.begin(0, 10, EasyCounter::COUNTER_MODE_DOWN);
  stunCounter.begin(0, 10, EasyCounter::COUNTER_MODE_DOWN);

  //initializes the audio player and sets the volume
  audio.begin(25);

  // initialize all the leds
  // initialize the trigger led and set brightness
  fireLed.begin(75);

  // set up the fire trigger and the debounce threshold
  trigger.begin(25);
}

/**
 *  The Main loop Routine
 *  1. Check if the front leds have been activated
 *    a. update the leds following the pattern that has been set
 *  2. Check the AmmoDown activation
 *    a. Toggles a clip counter
 *    b. Checks for an empty clip
 *    c. If clip is not empty
 *       i. queue audio track for selected fire mode
 *      ii. activate front led with blaster shot
 *          1. Flash white
 *          2. Burn Red to orange, then fade as it cools
 *    d. Otherwise queue empty clip track
 *  2. Check the ammo mode change
 *    a. Resets all counters
 *    b. Plays change mode audio track
 */
void loop(void) {
  powerUp();
  // Update the triggers LEDS in case they were activated. This should always be run in the main loop.
  fireLed.updateDisplay();
// check the trigger for input  
  checkTriggerSwitch();
}

/**
 * Playback a powerup track on start up. This can be called many times.
 * It will only play the track once.
 */
void powerUp(void) {
  if (playStartupSound) {
    DBGLN("Powering up");
    audio.playTrack(AUDIO_TRACK_START_UP);
    playStartupSound = 0;
  }
}

/**
 * Checks the fire trigger momentary switch.
 * Short press should send an alternating blaster pulse
 * Long press should change modes between A/B and C/D
 */
bool checkTriggerSwitch(void) {
  // check trigger button
  int buttonStateFire = trigger.checkState();
  // check if a trigger is pressed.
  if (buttonStateFire == EasyButton::BUTTON_PRESSED) {
    handleAmmoDown();
    return true;
  }
  if (buttonStateFire == EasyButton::BUTTON_LONG_PRESS) {
    setNextAmmoMode();
    return true;
  }
  return false;
}

/**
 *  Sends a blaster pulse.
 *    1. Toggles a clip counter
 *    2. Checks for an empty clip
 *       a. play empty clip track
 *    3. If clip is not empty
 *       a. queue audio track
 *       b. activate led strip
 *       c. activate oled refresh
 *       d. Check for low ammo
 */
void handleAmmoDown(void) {
  // move the counter
  bool emptyClip = !getTriggerCounter().tick();
  if (emptyClip) {
    //DBGLN(F("Empty clip"));
    audio.playTrack(getSelectedTrack(AMMO_MODE_IDX_EMTY));
    return;
  }
  //DBGLN(F("Ammo fire sequence"));
  //play the track
  // alternate between two firing blasts
  uint8_t idx = getTriggerCounter().getCount() % 2;
  audio.playTrack(getSelectedTrack(idx));
  // activate the led pulse
  //DBGLN(F("handleAmmo - activate leds"));
  fireLed.activate(blasterShot);
}

/**
 *  Change to the next ammo mode in the cycle
 *  1. change the selected ammo mode
 *  2. initialize the LED sequence
 *  3. queue playback
 *  4. set screen refresh
 */
void setNextAmmoMode(void) {
  // Increment the trigger mode index or reset to 0
  int mode = selectedTriggerMode + 1;
  if (mode == 2) mode = 0;
  changeAmmoMode(mode);
}

/**
 *  1. Set the selected ammo mode
 *  2. Initialize the LED sequence
 *  3. playback change mode
 *  4. reload ammo
 */
void changeAmmoMode(int mode) {
  if (mode > -1 && mode < 2) {
    selectedTriggerMode = mode;
    // Check for Switching modes
    if (selectedTriggerMode == AMMO_MODE_FIRE) {
      blasterShot.initialize(fireLed.RED, fireLed.ORANGE);  // shot - flash with color fade
      DBGLN("Fire Mode selected");
    }
    if (selectedTriggerMode == AMMO_MODE_STUN) {
      blasterShot.initialize(fireLed.YELLOW, fireLed.WHITE);  // shot - flash with color fade
      DBGLN("Stun Mode selected");
    }
    audio.playTrack(getSelectedTrack(AMMO_MODE_IDX_CHGE));
    reloadAmmo();
    delay(100);
  }
}


/**
 *  Routine for resetting the ammo counters
 *  1. all counters are reset
 *  2. leds are turned off
 *  3. queue audio track
 *  4. activate oled refresh
 */
void reloadAmmo(void) {
  //DBGLN(F("Reloading all counters"));
  fireCounter.resetCount();
  stunCounter.resetCount();
}

/**
 *  Routine for getting the selected trigger counter
 */
EasyCounter& getTriggerCounter() {
  if (selectedTriggerMode == AMMO_MODE_FIRE)
    return fireCounter;
  return stunCounter;
}

/**
 *  Convenience method for selecting a track to playback based on the selected
 *  trigger mode and a state variable
 */
uint8_t getSelectedTrack(uint8_t trackIdx) {
  return AUDIO_TRACK_AMMO_MODE_ARR[selectedTriggerMode][trackIdx];
}
