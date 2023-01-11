#ifndef blasterconfig_h
#define blasterconfig_h

/**
 * This file can be used to define the configuration of the circuit.
 * 
 * For each component that is enabled you should update the related 
 * configuration section.
 */

// To disable any component set value to 0
#define ENABLE_DEBUG            0 //Enable Debugging
#define ENABLE_EASY_AUDIO       1 //Enable audio
#define ENABLE_EASY_LED         1 //Enable LEDs
#define ENABLE_EASY_BUTTON      1 //Enable triggers

// Pin configuration for MP3 Player
#define AUDIO_TX_PIN        5
#define AUDIO_RX_PIN        4

// Pin configuration for all momentary triggers
#define TRIGGER_PIN         3

// Pin configuration for front barrel WS2812B LED
// set these to 0 if you want to disable the component
#define FIRE_LED_PIN          13
#define FIRE_LED_CNT          1

/**
 * Audio tracks by file index - upload these to the SD card in the correct order.
 * See the instructions on the Github page about loading the SD card.
 * 
 * You can reuse sound effects for each slot with two options:
 * 1. Copy the audio file in each slot where you want to use it
 * 2. Change the index values below to match the files on the SD Card
 */
static const int AUDIO_TRACK_START_UP          =   1;
static const int AUDIO_TRACK_AMMO_CHANGE_MODE  =   2;
static const int AUDIO_TRACK_AMMO_FIRE_A       =   3;
static const int AUDIO_TRACK_AMMO_FIRE_B       =   4;
static const int AUDIO_TRACK_AMMO_STUN_A       =   5;
static const int AUDIO_TRACK_AMMO_STUN_B       =   6;
static const int AUDIO_TRACK_AMMO_RELOAD       =   7;
static const int AUDIO_TRACK_AMMO_EMPTY        =   8;
static const int AUDIO_TRACK_THEME             =   9;


/**
 *  Common constant definitions - DO NOT CHANGE
 */
#define FASTLED_USE_PROGMEM 1

/**
 * Audio track index for lookup array. DO NOT CHANGE
 */
static const uint8_t AMMO_MODE_IDX_FIRE_1  =      0;  // fire
static const uint8_t AMMO_MODE_IDX_FIRE_2  =      1;  // fire
static const uint8_t AMMO_MODE_IDX_EMTY    =      2;  // empty clip
static const uint8_t AMMO_MODE_IDX_CHGE    =      3;  // change mode

/**
 * Lookup array for audio tracks based on ammo mode. DO NOT CHANGE
 */
static const uint8_t AUDIO_TRACK_AMMO_MODE_ARR[2][4]  =  {
  {AUDIO_TRACK_AMMO_FIRE_A, AUDIO_TRACK_AMMO_FIRE_B, AUDIO_TRACK_AMMO_EMPTY, AUDIO_TRACK_AMMO_CHANGE_MODE},
  {AUDIO_TRACK_AMMO_STUN_A, AUDIO_TRACK_AMMO_STUN_B, AUDIO_TRACK_AMMO_EMPTY, AUDIO_TRACK_AMMO_CHANGE_MODE}
};

/**
 * Audio track index for lookup array. DO NOT CHANGE
 */
static const uint8_t AMMO_MODE_FIRE = 1;
static const uint8_t AMMO_MODE_STUN = 2;

/**
 * DEBUG Macros
 */
#if ENABLE_DEBUG == 1
#define DBGSTR(message) Serial.print(message)
#define DBGBUF(buf, len) Serial.write(buf, len)
#define DBGLN(message) Serial.println(message)
#define DBGFMT(msg, fmt) Serial.print(msg, fmt)
#define DBGCHAR(c) Serial.write(c)
#else
#define DBG(message)
#define DBGSTR(message)
#define DBGBUF(buf, len)
#define DBGLN(message)
#define DBGFMT(msg, fmt)
#define DBGCHAR(c)
#endif  // ENABLE_DEBUG

#endif
