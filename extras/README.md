## Arduino Scripts 
There's a number of Arduino sketches to help you get up and running once you have your electronics ready to go and soldered up.

Directories:
 1. dfplayerpro_config - Load this sketch to help reconfig the 
 
### Configuring DF Player Pro module
This is experimental. We are testing new component for the audio module. The DF Player Pro (DF1201S) is a full featured mp3 player that includes a DAC, onboard 128M of memory, and onboard amp (PAM8302). This would replace three components used in the current build.

Out of the box, this module has a voice prompt that needs to be disabled. Run this script to disable the voice prompt.
 1. Open the dfplayerpro_config sketch
 2. Set the correct RX/TX pins numbers to match your wiring
 3. Upload the sketch to the Arduino
 4. Open the Serial Monitor. Set baud rate 115200
 5. Wait until it says Power Down and unplug
