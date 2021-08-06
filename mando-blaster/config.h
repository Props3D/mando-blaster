#ifndef blasterconfig_h
#define blasterconfig_h

/**
 * This file can be used to define the configuration of the circuit.
 * 
 * For each component that is enabled you should update the related 
 * configuration section.
 */

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
static const int TRACK_FIRE_ARR[]    = {TRACK_FIRE_A, TRACK_FIRE_B};
static const int TRACK_STUN_ARR[]    = {TRACK_STUN_A, TRACK_STUN_B};

// Common constant definitions
#define SELECTOR_FIRE_MODE 1
#define SELECTOR_STUN_MODE 2

#endif
