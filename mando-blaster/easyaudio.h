#ifndef easyaudio_h
#define easyaudio_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include "debug.h"

/**
 * EasyAudio is based on DF Player Mini, and provides simple setup, and easy track playback.
 * 
 * Constructor takes rx and tx pins as inputs, but will default to 0 and 1.
 * eg: EasyAudio audio(0, 1);
 * 
 * Call the begin function to initialize the serial comms, and set the volume.
 * Volume range is 0 - 30, default is 25.
 * eg.audio.begin(15);
 * 
 * Playback is by track index. The track index is determined by the order in which 
 * the files are loaded/copied onto the SD card.
 * eg. audio.playTrack(1);
 */
class EasyAudio
{
  private:
    SoftwareSerial mySerial;
    DFPlayerMini_Fast player;

  public:
    EasyAudio(uint8_t rxPin = 0, uint8_t txPin = 1) : mySerial(rxPin, txPin) {};

    void begin(uint8_t vol) {
#ifdef ENABLE_EASY_AUDIO
      debugLog("setup audio");
      mySerial.begin(9600);
      player.begin(mySerial, 100); //set Serial for DFPlayer-mini mp3 module 
      player.volume (vol);     //initial volume, 30 is max, 3 makes the wife not angry
#endif
    }

    void playTrack(int track) {
#ifdef ENABLE_EASY_AUDIO
      debugLog("playing track ");
      player.playFromMP3Folder( track ); 
#endif
    }
};

#endif
