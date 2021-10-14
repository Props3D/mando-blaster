## mando-blaster 1.0
This is a simple Arduino sketch for building electronic kits for various 3d printed
blasters using Ardunio components.

## Electonic Components

Electronic | Components
------------ | -------------
![Arduino Nano v3](https://user-images.githubusercontent.com/20442880/137374161-c34e9e7f-19bf-4b33-ba91-6ae4edc709ff.jpg) | ![DF Player Mini with Micro SD Card](https://user-images.githubusercontent.com/20442880/137374190-bcfa4d14-6f8d-4973-b493-c87bd1d0676d.jpg)
Arduino Nano v3 | DF Player Mini + 64mb Micro SD Card
![PAM8302A Amp](https://user-images.githubusercontent.com/20442880/137374587-abdad903-b008-4d2e-830e-c8b3c6a2c837.jpg) | ![Neopixel](https://user-images.githubusercontent.com/20442880/137407119-071466df-ffee-421e-b6c4-ed886e3be7d5.jpg)
Speaker Amp - PAM8302A | Single LED - Neopixel
![40mm 2W speaker](https://user-images.githubusercontent.com/20442880/137374750-e579754a-7173-41bb-a8a1-b5eaa5d234b1.jpg) | ![7mm Momentary Switch](https://user-images.githubusercontent.com/20442880/137374760-36ab96b2-bcd5-487a-a510-a3efed1216dc.jpg)
40mm 2W speaker | 7mm momentary switch
![Mini 360 DC-DC Buck Converter](https://user-images.githubusercontent.com/20442880/137374870-9687ea11-6a7c-48f6-8c0b-2ff4e34c62d3.jpg) | ![Lipo 7 4v](https://user-images.githubusercontent.com/20442880/137374882-cb61cde1-8c05-4817-9e06-7526e851bfad.jpg)
Mini 360 DC-DC Buck Converter | Lipo 7.4v
![1K Resister](https://user-images.githubusercontent.com/20442880/137374919-023f0bea-65f6-46a1-9d45-69f5a79e6916.jpg) | ![DRV2605L with 8mm ERM](https://user-images.githubusercontent.com/20442880/137374950-a2673b35-5a02-46b6-b322-de556f20e97e.jpg)
2 X 1K Resister | DRV2605L with 8mm ERM (Optional)

## Required Libraries
There's are number of libraries that you will need to install using the  Library Manager:
 1. DFPlayerMini_Fast
 2. FastLED
 4. FireTimer
 5. ezButton

## Setup and Configuration
The code can be used by updating the values in config.h based on your components,
wiring, and audio tracks.

```c++   
#define ENABLE_DEBUG               0

// Enable any items in your setup by uncommenting
#define ENABLE_EASY_AUDIO          1 //Enable all audio 
#define ENABLE_EASY_BUTTON         1 //Enable all buttons

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
```

## Audio File Setup
The DF Mini Player will play back both mp3 and wav files. However, the program play
back tracks based on index. The index is determined by the order the files are loaded
onto the card. It also expects all files to be loaded into a sub directory on the card
called "/mp3".

When using a MAC to load files, it will create hidden files that will cause the playback
to seem like it's not working. You'll need to use the terminal window to rm all of these
files and directories from the card.

## Example Wiring Diagram:
![docs/SampleWiring.png](docs/SampleWiring.png)
