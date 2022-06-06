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
#include "easyledv3.h"

/**
 * All components are controlled or enabled by "config.h". Before running, 
 * review and change all configurations based on your setup.
 * 
 * There's no need to change any of the following code or functions. 
 */
EasyAudio audio(AUDIO_RX_PIN, AUDIO_TX_PIN);
EasyButton trigger(TRIGGER_PIN);

EasyLedv3<SINGLE_LED_CNT, FIRE_LED_PIN> fireLed;
ezBlasterShot blasterShot(fireLed.RED, fireLed.ORANGE);  // initialize colors to starting fire mode

EasyCounter fireCounter("fire", TRACK_FIRE_ARR, TRACK_CLIP_EMPTY, TRACK_CLIP_RELOAD);
EasyCounter stunCounter("stun", TRACK_STUN_ARR, TRACK_CLIP_EMPTY, TRACK_CLIP_RELOAD);


// trigger mode status
uint8_t selectedTriggerMode   = SELECTOR_FIRE_MODE;   // sets the fire mode to blaster to start
bool playStartupSound          = 1;                    // play ower up sound on startup

// function declarations
void powerUp();
void handleFireTrigger();
void handleLedDisplay();
void handleSelectorMode(int state);
void sendBlasterPulse(EasyCounter &counter);
EasyCounter& getTriggerCounter();

void setup () {
  Serial.begin (9600);
  debugLog("Starting setup");
  // set up the fire trigger and the debounce threshold
  trigger.begin(25);

  //initializes the audio player and sets the volume
  audio.begin(25);

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
    debugLog("Powering up");
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
      sendBlasterPulse(getTriggerCounter());
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
void sendBlasterPulse(EasyCounter &counter) {
  debugLog("send alternating blaster pulse");
  bool emptyClip = counter.isEmpty();
  bool fullClip = counter.isFull();
  // check the track number before ticking, in case it's the last round
  // alternate between two firing blasts
  uint8_t idx = counter.getCount() % 2;
  int track = counter.getTrackNumber(idx); // returns the approapriate track, or empty clip
  counter.tick();
  if (fullClip)
    track = counter.getTrackNumber(idx);   // We redo this when the clip is full to get the right track
  audio.playTrack(track);
  if (emptyClip == false) {
    fireLed.activate(blasterShot);
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
    blasterShot.initialize(fireLed.YELLOW, fireLed.WHITE);  // shot - flash with color fade
    debugLog("Stun Mode selected");
  } else {
    selectedTriggerMode = SELECTOR_FIRE_MODE;
    blasterShot.initialize(fireLed.RED, fireLed.ORANGE);  // shot - flash with color fade
    debugLog("Fire Mode selected");
  }
  audio.playTrack(TRACK_CHANGE_MODE);
  delay (100);
}
