#ifndef easycounter_h
#define easycounter_h

#include <Arduino.h>
#include "debug.h"

const static int COUNTER_MODE_UP = 1;
const static int COUNTER_MODE_DOWN = -1;
/**
 * Simple class for tracking counts up or down.
 * Use the constructor to set the min, max, and increment.
 * eg. EasyCounter countZeroToOneHundredByOne(0, 100, 1);
 * eg. EasyCounter countZeroToOneHundredByTwo(0, 100, 2);
 * eg. EasyCounter countOneHundredToZeroByOne(0, 100, -1);
 * eg. EasyCounter countOneHundredToZeroByTwo(0, 100, -2);
 * 
 * You can move the counter by using the toggleCount function:
 * eg. int value = countZeroToOneHundredByOne.toggleCount();
 * 
 * You can reset the counter to the original state:
 * eg. int value = countZeroToOneHundredByOne.resetCount();

 */
class EasyCounter
{
  private:
    String name;
    int8_t increment = COUNTER_MODE_DOWN;
    uint8_t low;
    uint8_t high;
    uint8_t currentCounter;
    // file indexes for different states
    uint8_t activeTrackIdx[2];
    uint8_t emptyTrackIdx;
    uint8_t resetTrackIdx;
 
    // helper functions
    void setLow(int number) { this->low = number; }
    void setHigh(int number) { this->high = number; }
    void setIncrement(int value) { this->increment = value; }
    void setCount(int value) { this->currentCounter = value; }
  public:
    EasyCounter(const char *label) : name(label) {}
    EasyCounter(const char *label, uint8_t activeTracks[], uint8_t emptyTrack, uint8_t resetTrack) : name(label) {
      this->activeTrackIdx[0] = activeTracks[0];
      this->activeTrackIdx[1] = activeTracks[1];
      this->emptyTrackIdx = emptyTrack;
      this->resetTrackIdx = resetTrack;
    }

    void begin(uint8_t lowNumber, uint8_t highNumber, int8_t increment) {
      debug.log(String("Initialing counter: ") + name);
      setLow(lowNumber);
      setHigh(highNumber);
      setIncrement(increment);
      resetCount();
    }  

    EasyCounter* EasyCounter::tick() {
      if (isEmpty()) return this;
      if (increment == COUNTER_MODE_UP) 
          currentCounter = currentCounter + 1;
       else
          currentCounter = currentCounter - 1;
      return this;
    }

    bool isEmpty() {
      if (increment == COUNTER_MODE_UP)
        return (currentCounter == high);
      if (increment == COUNTER_MODE_DOWN)
        return (currentCounter == low);
      return false;
    }

    bool isFull() {
      if (increment == COUNTER_MODE_UP)
        return (currentCounter == low);
      if (increment == COUNTER_MODE_DOWN)
        return (currentCounter == high);
      return false;
    }

    uint8_t resetCount() {
      if (increment == COUNTER_MODE_UP)
        setCount(low);
      if (increment == COUNTER_MODE_DOWN)
        setCount(high);
      debug.log(name + String(" reset counter: ") + String(currentCounter));
      return this->currentCounter;
    }

    uint8_t getCount() {
      return this->currentCounter;
    }

    uint8_t getTrackNumber() {
      if (isEmpty()) return this->emptyTrackIdx;
      if (isFull()) return this->resetTrackIdx;
      return this->activeTrackIdx[0];
    }

    uint8_t getTrackNumber(int index) {
      if (isEmpty()) return this->emptyTrackIdx;
      if (isFull()) return this->resetTrackIdx;
      if (index == 1)
        return this->activeTrackIdx[index];
      return this->activeTrackIdx[0];
    }
};

#endif
