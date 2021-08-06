## mando-blaster 1.0
This is a simple Arduino sketch for building electronic kits for various 3d printed
blasters using Ardunio components.

## Arduino Components
Arduino Nano v3
DF Player Mini
Speaker Amp - PAM8302A
Single LED - Neopixel
40mm 2W speaker
7mm momentary switch
5v Regulator - LM2596
Lipo 7.4v


## Setup and Configuration
The code can be used by updating the values in config.h based on your components,
wiring, and audio tracks.

```c++   

// Enable debug logging
// enabled == 1
// disabled == 0
#define ENABLE_DEBUG        1

// Enable any items in your setup by uncommenting
#define ENABLE_AUDIO                 1

// Pin configuration for MP3 Player
#define AUDIO_RX_PIN        10
#define AUDIO_TX_PIN        11

// Pin configuration for all momentary triggers
#define TRIGGER_PIN         8

// Pin configuration for front barrel WS2812B LED
#define SINGLE_LED_CNT      1
#define FIRE_LED_PIN        13 

// track by file index
#define TRACK_START_UP        1
#define TRACK_CHANGE_MODE     2
#define TRACK_FIRE_A          3
#define TRACK_FIRE_B          4
#define TRACK_STUN_A          5
#define TRACK_STUN_B          6
#define TRACK_CLIP_RELOAD     7
#define TRACK_CLIP_EMPTY      8
#define TRACK_THEME           9

// Common constant definitions - DO NOT CHANGE
#define SELECTOR_FIRE_MODE 1
#define SELECTOR_STUN_MODE 2

static const int TRACK_FIRE_ARR[]    = {TRACK_FIRE_A, TRACK_FIRE_B};
static const int TRACK_STUN_ARR[]    = {TRACK_STUN_A, TRACK_STUN_B};

```

## Example Wiring Diagram:
![SampleWiring.jpg]

