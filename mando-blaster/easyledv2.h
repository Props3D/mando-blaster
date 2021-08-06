#ifndef easyledv2_h
#define easyledv2_h

#include <Arduino.h>
#include <FastLED.h>
#include "debug.h"



/**
 * A simple class for managing a collection of WS2812 leds. It has three configurations:
 * 1. Single Led
 * 2. Led Strip
 * 3. Led Strip with single led
 * 
 * Use the delcaration to specify the LED configuration at compile time:
 * eg. EasyLeds<LED_STRIP_PIN, NUM_LEDS, SINGLE_LED_PIN> leds;
 * 
 * In the setup, you can use the begin() function to initialize the brightness.
 * eg. leds.begin(75);
 * 
 * Use various functions to display different patterns when using LED strip.
 * Fast Fire - red
 * eg. leds.fire();
 * Pulsing Stun - blue
 * eg. leds.stun();
 * 
 * REQUIRED LIBRARY: FastLED
 */

template <uint8_t LED_COUNT, int LED_PIN_IN>
class EasyLedv2
{
  private:
    // variable declaration
    CRGB leds[LED_COUNT];
    CRGB currentColor;  // starting color
    CRGB targetColor;   // target color
    const uint8_t fadeRate = 42;   // larger number is a slower fade
    boolean fadeToColor = 0;       // turns on/off the fading toward target
    uint8_t blendRate = 2;   // larger number is a slower fade

    /** function declaration **/
    // Apply LED color changes
    void showStrip() {
      FastLED.show();
    }

    // Set a LED color (not yet visible)
    void setPixel(int Pixel, byte red, byte green, byte blue) {
      if (Pixel < LED_COUNT) {
        leds[Pixel].r = red;
        leds[Pixel].g = green;
        leds[Pixel].b = blue;
      }
    }
  public:
    //some constants for functions
    const CRGB BLACK = CRGB::Black;
    const CRGB RED = CRGB::Red;
    const CRGB BLUE = CRGB::Blue;
    const CRGB GREEN = CRGB::Green;
    const CRGB ORANGE = CRGB(255, 95,0); //darker orange
    const CRGB YELLOW = CRGB::Yellow;
    const CRGB PURPLE = CRGB::Purple;
    const CRGB WHITE = CRGB::White;

    EasyLedv2() : currentColor(0,0,0), targetColor(0,0,0) {};
    
    void begin(int brightness) {
      if (LED_COUNT > 0 && LED_PIN_IN > 0) {
        FastLED.addLeds<WS2812, LED_PIN_IN, GRB>(leds, LED_COUNT);
        FastLED.setBrightness(brightness);
        FastLED.setMaxPowerInVoltsAndMilliamps(5, 500); //5v and 500mA
      }
    }

    //===============================================================
    void blasterShot(CRGB startColor, CRGB endColor) {
      if(LED_COUNT > 0 && LED_PIN_IN > 0) {
        debug.log("starting blaster shot");
        currentColor = CRGB(startColor.r, startColor.g, startColor.b);
        targetColor = CRGB(endColor.r, endColor.g, endColor.b);
        fadeToColor = 2;  // start fading toward target
        blendRate = 2;
        fill_solid(leds, LED_COUNT, WHITE);
        showStrip();
        delay (100); //Delay before next shot so the LED won't stay on
      }
    }

    void updateDisplay() {
      if(LED_COUNT > 0 && LED_PIN_IN > 0) {
        EVERY_N_MILLISECONDS(fadeRate) {
          // stop fading
          if (fadeToColor && currentColor == BLACK) {
            debug.log("ending blaster shot");
            fadeToColor = 0;
          }
          // fade to black
          if (fadeToColor && currentColor == targetColor) {
            debug.log("fade to black on blaster shot");
            // on match, we set to target to black to fade out
            targetColor = CRGB(0, 0, 0); // setto black
            fadeToColor = 1;  // phase 2
            blendRate = 32;   // faster fade to black
          }
          // fade to target color
          if (fadeToColor) {
            //debug.log("blending blaster shot");
            fadeTowardColor(currentColor, targetColor, blendRate);
            fill_solid(leds, LED_COUNT, currentColor);
            blendRate = blendRate+1;
          }
          showStrip();
        }
        // double blend speed after every 5 executions
        EVERY_N_MILLISECONDS(fadeRate*5) {
          if (fadeToColor) {
            blendRate = blendRate*2; 
          }
        }
      }
    } // end_main_loop
    
    // Helper function that blends one uint8_t toward another by a given amount
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

    // Blend one CRGB color toward another CRGB color by a given amount.
    // Blending is linear, and done in the RGB color space.
    // This function modifies 'cur' in place.
    CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount) {
      nblendU8TowardU8( cur.red,   target.red,   amount);
      nblendU8TowardU8( cur.green, target.green, amount);
      nblendU8TowardU8( cur.blue,  target.blue,  amount);
      return cur;
    }

    void fadeInOut(byte red, byte green, byte blue) {
      float r, g, b;
         
      for(int k = 0; k < 256; k=k+1) {
        r = (k/256.0)*red;
        g = (k/256.0)*green;
        b = (k/256.0)*blue;
        setAll(r,g,b);
        showStrip();
      }
         
      for(int k = 255; k >= 0; k=k-2) {
        r = (k/256.0)*red;
        g = (k/256.0)*green;
        b = (k/256.0)*blue;
        setAll(r,g,b);
        showStrip();
      }
    }

    void leftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
      if(LED_COUNT > 0 && LED_PIN_IN > 0) {
        for(int i = 0; i < LED_COUNT-EyeSize-2; i++) {
          setAll(0,0,0);
          setPixel(i, red/10, green/10, blue/10);
          for(int j = 1; j <= EyeSize; j++) {
            setPixel(i+j, red, green, blue);
          }
          setPixel(i+EyeSize+1, red/10, green/10, blue/10);
          showStrip();
          delay(SpeedDelay);
        }
        delay(ReturnDelay);
      }
    }
};

#endif
