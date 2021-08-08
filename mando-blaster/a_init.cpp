/*
 * name:  ez-blaster
 * Author:  FriskyDingo
 * Date:  2021-07-14
 * Description: Universal code for various blaster setups
 */
#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include "easybutton.h"
#include "easycounter.h"
#include "easyaudio.h"
#include "easyledv2.h"

/**
 * All components are controlled or enabled by "config.h". Before running, 
 * review and change all configurations based on your setup.
 * 
 * There's no need to change any of the following code or functions. 
 */
EasyAudio audio(AUDIO_RX_PIN, AUDIO_TX_PIN);
EasyButton trigger(TRIGGER_PIN);
EasyLedv2<SINGLE_LED_CNT, FIRE_LED_PIN> fireLed;
EasyCounter fireCounter("fire", TRACK_FIRE_ARR, TRACK_CLIP_EMPTY, TRACK_CLIP_RELOAD);
EasyCounter stunCounter("stun", TRACK_STUN_ARR, TRACK_CLIP_EMPTY, TRACK_CLIP_RELOAD);


// trigger mode status
int selectedTriggerMode   = SELECTOR_FIRE_MODE;   // sets the fire mode to blaster to start
int playStartupSound      = 1;           // startup sound variable

// function declarations
void powerUp();
void handleFireTrigger();
void handleLedDisplay();
void handleSelectorMode(int state);
void sendBlasterPulse(EasyCounter &counter, int waitTime);
EasyCounter& getTriggerCounter();

void setup () {
  Serial.begin (9600);
  debug.log("Starting setup");
  // set up the fire trigger and the debounce threshold
  trigger.begin(25);

  //initializes the audio player and sets the volume
  audio.begin(20);

  // initialize all the leds
  // initialize the trigger led and set brightness
  fireLed.begin(75);

  // Initialize the clip counters for different modes
  fireCounter.begin(0, 10, COUNTER_MODE_DOWN);
  stunCounter.begin(0, 10, COUNTER_MODE_DOWN);
}

void loop () {
  powerUp();
  handleLedDisplay();
  handleFireTrigger();
}

/**
 * Playback a powerup track on start up. This can be called many times.
 * It will only play the track once.
 */
void powerUp() {
  if (playStartupSound) {
    debug.log("Powering up");
    audio.playTrack(TRACK_START_UP);
    playStartupSound = 0;
  }
}

/**
 * Checks the fire trigger momentary switch.
 * Short press should send an alternating blaster pulse
 * Long press should change modes between A/B and C/D
 */
void handleFireTrigger() {
  // check trigger button
  int buttonStateFire = trigger.checkState();
  // check if a trigger is pressed.
  if (buttonStateFire == BUTTON_SHORT_PRESS) {
      sendBlasterPulse(getTriggerCounter(), 100);
  }
  if (buttonStateFire == BUTTON_LONG_PRESS) {
    handleSelectorMode(buttonStateFire);
  }
}

void handleLedDisplay() {
  fireLed.updateDisplay();
}
/**
 * Sends a blaster pulse.
 *   1. Toggles a clip counter
 *   2. Checks for an empty clip 
 *   3. If clip is not empty
 *      a. play audio track
 *      b. update led strip
 *      c. update led counter to the current clip counter
 *   4. Otherwise play empty clip track
 *
 */
void sendBlasterPulse(EasyCounter &counter, int waitTime) {
  debug.log("send alternating blaster pulse");
  // alternate between two firing blasts
  bool emptyClip = counter.isEmpty();
  uint8_t idx = counter.toggleCount() % 2;
  int track = counter.getTrackNumber(idx);
  audio.playTrack(track);
  if (emptyClip == false) {
    if (selectedTriggerMode == SELECTOR_FIRE_MODE) {
      fireLed.blasterShot(fireLed.RED, fireLed.ORANGE);
    } else {
      fireLed.blasterShot(fireLed.YELLOW, fireLed.WHITE);
    }
  }
}

EasyCounter& getTriggerCounter() {
  if (selectedTriggerMode == SELECTOR_FIRE_MODE)
      return fireCounter;
  return stunCounter;
}

/**
 * Checks the selector mode momentary switch.
 * 1. if a long preess plackback the theme track
 * 2. If a short press
 *   a. playback changee mode track
 *   b. toggle fire mode
 */
void handleSelectorMode(int buttonStateMode) {
  // Check for Switching modes
  if (selectedTriggerMode == SELECTOR_FIRE_MODE) {
    selectedTriggerMode = SELECTOR_STUN_MODE;
    debug.log("Stun Mode selected");
  } else {
    selectedTriggerMode = SELECTOR_FIRE_MODE;
    debug.log("Fire Mode selected");
  }
  audio.playTrack(TRACK_CHANGE_MODE);
  delay (100);
}
