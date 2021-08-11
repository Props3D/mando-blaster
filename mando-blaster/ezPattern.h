#ifndef ezpattern_h
#define ezpattern_h

#include <FastLED.h>
#include "debug.h"

typedef void (*callback_function)(void); // type for conciseness

/**
 * A set of classes for producing LED patterns.
 * 
 * Each pattern has it's own constructor. Some exmaples:
 *    ezBlasterShot hotshot(CRGB:Red, CRGB::Orange); // red fade to orange
 *    ezBlasterPulse bluepulse(CRGB:Blue, 2);        // 2 pixel blue animation
 *    ezBlasterPulse bluepulse(CRGB:Blue, 2);        // 2 pixel blue animation
 *
 * A pattern is passed to an EasyLedv3 to control the LED set.
 * e.g. leds.activate(hotshot);
 * 
 * REQUIRED LIBRARY: FastLED
 */
 
/**
 * Interface for building different LED patterns and behaviours
 */
class ezPattern {
  protected:
    callback_function callbackPtr = 0;     // pointer to callback function
    uint8_t activated             = 0;     // signal when the pattern should be active
    uint8_t frameRate             = 16;    // larger number is a slower fade
    unsigned long flashTimer      = 0;     // time when the white flash started

    static const uint8_t flashDuration = 75;    // larger number will hold a white flash longer
    static const uint8_t delta         = 1;     // Sets forward or backwards direction amount.
    static const uint8_t fadeRate      = 220;   // How fast to fade out tail. [0-255]

    // fucntion declartions
    void show() {
      FastLED.show();
    }
    void clear(CRGB *leds, uint8_t count) {
      fill_solid(leds, count, CRGB::Black);
    }
    void completed(struct CRGB *leds, uint8_t count) {
      clear(leds, count);
      if (callbackPtr) callbackPtr();
    }
    void whiteflash(CRGB *leds, int count) {
      flashTimer = millis(); // capture the time for timing the flash
      fill_solid(leds, count, CRGB::White);
      show();
    }
    void fadeToBlack(CRGB *leds, int count) {
      fadeToBlackBy(leds, count, fadeRate);
    }
  public:
    virtual void activate(CRGB *leds, uint8_t count) = 0;
    virtual void updateDisplay(CRGB *leds, uint8_t count) = 0;
    virtual ~ezPattern() = default;
};

/**
 * Single blaster shot
 * 1. Flash White
 * 2. Starting color with fade to secondary color
 * 3. fade to black to show cooling
 */
class ezBlasterShot : public ezPattern
{
  private:
    CRGB currentColor;  // color
    CRGB startColor;    // starting color
    CRGB targetColor;   // target color
    const CRGB coolOffColor = CRGB::Black;

    // processing variables
    uint8_t blendRate = 2;    // larger number will be a faster color blend

    // helper functions
    bool checkShotCooled(CRGB *leds, uint8_t count) {
      if ((activated == 1) && (currentColor == coolOffColor)) {
        activated = 0;
        this->completed(leds, count);
        this->show();
        return true;
      }
      return false;
    }
    bool checkShotBlended(CRGB *leds, uint8_t count) {
      if ((activated == 2) && (currentColor == targetColor)) {
        // on match, we set to target to black to fade out
        activated = 1;  // phase 2
        blendRate = 32;   // faster fade to black
        return true;
      }
      return false;
    }
    bool coolingShot(CRGB *leds, uint8_t count) {
      if (activated == 1) {
        fadeTowardColor(currentColor, coolOffColor, blendRate);
        fill_solid(leds, count, currentColor);
        blendRate = blendRate+1;
        this->show();

        // double blend speed after every 5 executions to quicken the transition
        // This will make the fading non-linear, but it's matter of timing on the effect
        EVERY_N_MILLISECONDS(frameRate*5) {
            blendRate = blendRate*2; 
        }
        return true;
      }
      return false;
    }
    bool blendingShot(CRGB *leds, uint8_t count) {
      if (activated == 2) {
        fadeTowardColor(currentColor, targetColor, blendRate);
        fill_solid(leds, count, currentColor);
        blendRate = blendRate+1;
        this->show();

        // double blend speed after every 5 executions to quicken the transition
        // This will make the fading non-linear, but it's matter of timing on the effect
        EVERY_N_MILLISECONDS(frameRate*5) {
            blendRate = blendRate*2; 
        }
        return true;
      }
      return false;
    }
    bool checkWhiteFlash(CRGB *leds, uint8_t count) {
      if (activated == 3) {
        long duration = millis() - flashTimer;
        if (duration > flashDuration)
          activated = 2; // start the fade
        return true;
      }
      return false;
    }
    /**
     * Blend one CRGB color toward another CRGB color by a given amount.
     * Blending is linear, and done in the RGB color space.
     * 
     * NOTE: This function modifies 'cur' color in place, and returns the same object.
     */
    CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount) {
      nblendU8TowardU8( cur.red,   target.red,   amount);
      nblendU8TowardU8( cur.green, target.green, amount);
      nblendU8TowardU8( cur.blue,  target.blue,  amount);
      return cur;
    }
    /**
     *  Helper function that blends one uint8_t toward another by a given amount
     */
    void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount) {
      if( cur == target) return;
      
      if( cur < target ) {
        uint8_t delta = target - cur;
        delta = scale8_video( delta, amount);
        cur += delta;
      } else {
        uint8_t delta = cur - target;
        delta = scale8_video( delta, amount);
        cur -= delta;
      }
    }
  public:
    ezBlasterShot(CRGB initialColor, CRGB endColor, callback_function callback = 0) {
      initialize(initialColor, endColor);
      callbackPtr = callback;
      frameRate = 16;
    }
    ~ezBlasterShot() {
        callbackPtr = 0;
    }

    void initialize(CRGB initialColor, CRGB endColor) {
      startColor = CRGB(initialColor.r, initialColor.g, initialColor.b);
      targetColor = CRGB(endColor.r, endColor.g, endColor.b);
    }

    void activate(CRGB *leds, uint8_t count) {
      debugLog("BlasterShot - initialized");
      currentColor = CRGB(startColor.r, startColor.g, startColor.b);
      activated = 3;    // white flash and color fade
      blendRate = 2;    // reset blend rate
      this->whiteflash(leds, count);
    }

    void updateDisplay(CRGB *leds, uint8_t count) {
      EVERY_N_MILLISECONDS(frameRate) {
        // stop fading and clear
        if (checkShotCooled(leds, count)) {
            debugLog("BlasterShot - ending blaster shot");
        }
        // fade to black
        if (checkShotBlended(leds, count)) {
          debugLog("BlasterShot - fade to black on blaster shot");
        }
        if (coolingShot(leds, count)) {
          debugLog("BlasterShot - Cooling off blaster shot");
        }
        if (blendingShot(leds, count)) {
          debugLog("BlasterShot - blending colors on blaster shot");
        }
        if (checkWhiteFlash(leds, count)) {
          debugLog("BlasterShot - checking white flash");
        }
      }
    }
};

/**
 * Send a blaster pulse down a set of leds. Supports mirrored mode.
 * ie. two equal strips connected in series.
 * 
 * blaster pulse sequence
 * 1. single color
 * 2. Defined width of how many leds to light up
 * 3. Animate across all leds
 * 4. Trailing fade to black to show cooling
 * 
 * Constructor parameters:
 * 1. color - led color
 * 2. width - how many pixles to light up in the pulse
 * 3. cbtpr - pointer to callback function. Useful for chaining actions together
 * 4. mirrored - boolean is strip mirrored in series or not
 * 5. frame rate - how fast to animate pulse, in millis
 */
class ezBlasterPulse : public ezPattern
{
  private:
    CRGB color;                          // led color
    uint8_t width = 2;                   // Can increase the number of pixels (width) of the chase. [1 or greater]

    uint8_t patternWidth = 0; // Total width of the pattern to animate
    int8_t advanceUp   = 0;   // Stores the starting position at the beginning of the strip.
    int8_t advanceDown = 0;   // Stores the starting position at the top of strip, when mirrored
    bool mirrored   = 1;      // is this a mirrored led strip
    int8_t middle   = 0;      // Where's the middle of the strip

    // helper functions
    bool checkPulseFinshed(CRGB *leds, uint8_t count) {
      uint8_t endIdx = mirrored ? middle : count;
      if (activated && ((advanceUp-patternWidth) > endIdx)) {
        activated = 0;
        return true;
      }
      return false;
    }
    bool checkPulseReachedTop(CRGB *leds, uint8_t count) {
      uint8_t endIdx = mirrored ? middle : count;
      if (activated && (advanceUp == (endIdx-1))) {
        this->completed(leds, count);
        return true;
      }
      return false;
    }
    bool advancePulse(CRGB *leds, uint8_t count) {
      if (activated) {
        // fade any previous leds to create a color trail
        this->fadeToBlack(leds, count);
        this->advancePulseUp(leds, count);
        if (mirrored)
          this->advancePulseDown(leds, count);
        this->show();
        return true;
      }
      return false;
    }
    bool advancePulseUp(CRGB *leds, uint8_t count) {
      // Advance pixel postion up  the strip, rolling off the end
      advanceUp = (advanceUp + delta);
      // determine the end if we are mirrored or not
      uint8_t endIdx = mirrored ? middle : count;
      // Update pixels down first 1/2 strip.
      for (uint8_t w = 0; w < width; w++) {
        int16_t pos = (advanceUp - w);
        if (pos > -1 && pos < endIdx)
          leds[pos] = color;
      }
    }
    bool advancePulseDown(CRGB *leds, uint8_t count) {
      // Advance pixel postion down the strip, rolling off the end
      advanceDown = (advanceDown - delta);
      // determine the end if we are mirrored or not
      uint8_t endIdx = mirrored ? middle : 0;
      // Update pixels down second the strip.
      for (uint8_t w = 0; w < width; w++) {
        int16_t pos = (advanceDown + w);
        if (pos < count && pos >= endIdx)
          leds[pos] = color;
      }
    }
  public:
    ezBlasterPulse(CRGB initialColor, uint8_t eyeWidth, callback_function callback = 0, bool mirroredStrip = 0, uint8_t frameSpeed = 20) {
      initialize(initialColor, eyeWidth);
      callbackPtr = callback;
      mirrored = mirroredStrip;
      frameRate = frameSpeed;
    }
    ~ezBlasterPulse() {
        callbackPtr = 0;
    }

    void initialize(CRGB initialColor, uint8_t eyeWidth) {
      color = CRGB(initialColor.r, initialColor.g, initialColor.b);
      width = eyeWidth;
      patternWidth = width;
    }
    void activate(CRGB *leds, uint8_t count) {
      debugLog("Blaster Pulse - initialized");
      advanceUp = 0;
      advanceDown = count-1;
      middle = count/2;
      activated = 1;
    }

    void updateDisplay(CRGB *leds, uint8_t count) {
      // Advance pixels to next position.
      EVERY_N_MILLISECONDS(frameRate) {
        if (checkPulseFinshed(leds, count)) {
          this->fadeToBlack(leds, count);
          debugLog("Blaster Pulse - ending blaster pulse");
        }
        if (checkPulseReachedTop(leds, count)) {
          debugLog("Blaster Pulse - reached the end of strip");
        }
        advancePulse(leds, count);
      }
    }

};

/**
 * Repeating blaster pulse animated across all leds. Trailing fade to black to show cooling.
 * 1. color - single led color
 * 2. width - Define width of how many leds to light up in a single pulse
 * 3. cbk - pointer to callback method. Can be used for chaining actions
 * 4. repeats - number of pulses to send
 * 5. spacing - number of leds between each pulse
 * 
  * Constructor parameters:
 * 1. color - led color
 * 2. width - how many pixles to light up in the pulse
 * 3. cbtpr - pointer to callback function. Useful for chaining actions together
 * 4. mirrored - boolean is strip mirrored in series or not
 * 5. frame rate - how fast to animate pulse, in millis
 * 6. trailingFade - do the leds fade behind the pulse or not
 */
class ezBlasterRepeatingPulse : public ezPattern
{
  private:
    CRGB color;                    // led color
    uint8_t width = 2;             // Can increase the number of pixels (width) of the chase. [1 or greater]
    uint8_t repeats = 2;           // number of pulses
    uint8_t space = 1;             // space between pulses

    uint8_t patternWidth = 0;   // Total width of the pattern to animate
    int8_t advanceUp     = 0;   // Stores the starting position at the beginning of the strip.
    int8_t advanceDown   = 0;   // Stores the starting position at the top of strip, when mirrored
    bool mirrored        = 0;   // is this a mirrored led strip
    uint8_t middle       = 0;   // Where's the middle of the strip
    bool fadePulse       = 1;   // Is there a trailing fade

    static const uint8_t delta         = 1;     // Sets forward or backwards direction amount.
    static const uint8_t fadeRate      = 220;   // How fast to fade out tail. [0-255]

    // helper fucntions
    bool checkPulseFinshed(CRGB *leds, uint8_t count) {
      uint8_t endIdx = mirrored ? middle : count;
      if (activated && ((advanceUp-patternWidth) > endIdx)) {
        activated = 0;
        return true;
      }
      return false;
    }
    bool checkPulseReachedTop(CRGB *leds, uint8_t count) {
      uint8_t endIdx = mirrored ? middle : count;
      if (activated && (advanceUp == (endIdx-1))) {
        this->completed(leds, count);
        return true;
      }
      return false;
    }
    bool advancePulse(CRGB *leds, uint8_t count) {
      if (activated) {
        if (fadePulse) {
          // fade any previous leds to create a color trail
          this->fadeToBlack(leds, count);
        } else {
          //clear the previous pattern or fade?
          this->clear(leds, count);
        }

        advancePulseUp(leds, count);
        if (mirrored)
          advancePulseDown(leds, count);
        this->show();
        return true;
      }
      return false;
    }
    bool advancePulseUp(CRGB *leds, uint8_t count) {
      // Advance pixel postion up  the strip, rolling off the end
      advanceUp = (advanceUp + delta);

      // determine the end if we are mirrored or not
      uint8_t endIdx = mirrored ? middle : count;
      // Advance the pattern down the strip.
      for (uint8_t r = 0; r < repeats; r++) {
        // find the starting index for each pulse
        uint8_t startIdx = r*(width+space);
        // set the color of each pulse
        for (uint8_t w = 0; w < width; w++) {
          int8_t pos = (advanceUp - (startIdx+w));
          // only set the color if position is within range, otherwise ignore
          if (pos > -1 && pos < endIdx)
            leds[pos] = color;
        }
        // black out between pulses
        if ((r+1) < repeats) {
          for (uint8_t w = 0; w < space; w++) {
            int8_t pos = (advanceUp - (startIdx+width+w));
            // only set the color if position is within range, otherwise ignore
            if (pos > -1 && pos < endIdx)
              leds[pos] = CRGB::Black;
          }
        }
      }
    }
    bool advancePulseDown(CRGB *leds, uint8_t count) {
      // Advance pixel postion down the strip, rolling off the end
      advanceDown = (advanceDown - delta);

      // determine the end if we are mirrored or not
      uint8_t endIdx = mirrored ? middle : 0;

      // Advance the pattern down the strip.
      for (uint8_t r = 0; r < repeats; r++) {
        // find the starting pattern index for each pulse
        uint8_t startIdx = r*(width+space);
        // set the color of each pulse
        for (uint8_t w = 0; w < width; w++) {
          int8_t pos = (advanceDown + (startIdx+w));
          // only set the color if position is within range, otherwise ignore
          if (pos < count && pos >= endIdx)
            leds[pos] = color;
        }
        // black out between pulses
        if ((r+1) < repeats) {
          for (uint8_t w = 0; w < space; w++) {
            int8_t pos = (advanceDown + (startIdx+width+w));
            // only set the color if position is within range, otherwise ignore
            if (pos < count && pos >= endIdx)
              leds[pos] = CRGB::Black;
          }
        }
      }
    }
  public:
    ezBlasterRepeatingPulse(CRGB initialColor, uint8_t eyeWidth = 2, bool mirroredStrip = 0, callback_function callback = 0, uint8_t repeating = 2, uint8_t spacing = 2, uint8_t frameSpeed = 20, bool trailingFade = 0) {
      initialize(initialColor, eyeWidth, repeating, spacing, trailingFade);
      mirrored = mirroredStrip;
      callbackPtr = callback;
      frameRate = frameSpeed;
    }
    ~ezBlasterRepeatingPulse() {
        callbackPtr = 0;
    }

    void initialize(CRGB initialColor, uint8_t eyeWidth, uint8_t repeating, uint8_t spacing, bool trailingFade) {
      color = CRGB(initialColor.r, initialColor.g, initialColor.b);
      width = eyeWidth;
      repeats = repeating;
      space = spacing;
      patternWidth = (width+space)*repeats-space;
      fadePulse = trailingFade;
    }

    void activate(CRGB *leds, uint8_t count) {
      debugLog("RepeatPulse - initialized");
      advanceUp = 0;
      advanceDown = count-1;
      middle = count/2;
      activated = 1;
    }

    void updateDisplay(CRGB *leds, uint8_t count) {
      // Advance pixels to next position.
      EVERY_N_MILLISECONDS(frameRate) {
        if (checkPulseFinshed(leds, count)) {
          this->fadeToBlack(leds, count);
          debugLog("Repeater Pulse - ending blaster pulse");
        }
        if (checkPulseReachedTop(leds, count)) {
          debugLog("Repeater Pulse - reached the end of strip");
        }
        advancePulse(leds, count);
      }
  }
};

#endif
