/**
 * This is the main file for the mando blaster setup.
 * 
 * All components settings can be found in "config.h". Before running, 
 * review and change all configurations based on your setup.
 * 
 * There's are number of libraries that you will need to install using the 
 * Library Manager:
 * 2. FastLED
 * 5. ezButton
 * 
 * The main loop:
 * 1. Playback power up - on start up only
 * 2. Check the fire trigger momentary switch.
 * 3. Send a blaster pulse, if fire trigger is a short press
 *   a. Toggles a clip counter
 *   b. Checks for an empty clip 
 *   c. If clip is not empty
 *      i. play alternating audio tracks for different sounds 
 *     ii. activate front led with blaster shot
 *         1. Flash white
 *         2. Burn Red to orange, then fade as it cools
 *   d. Otherwise play empty clip track
 * 4. Changes mode, if fire trigger is a long press
 *     i. playback change mode track
 *    ii. toggle fire mode
 * 
 */
 
