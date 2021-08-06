#pragma once 

#ifndef debuglog_h
#define debuglog_h

#include <Arduino.h>

/**
 * Debug logging that can be disabled
 */
template<uint8_t ENABLE_LOGGING>
class DebugLog
{
  private:
    const uint8_t ENABLED = ENABLE_LOGGING;

  public:
    void log(const String &msg) {
      if (ENABLED) Serial.println(msg);
    }
    
    void log(int msg, int base) {
      if (ENABLED) Serial.println(msg, base);
    }
    
    void log(long msg, int base) {
      if (ENABLED) Serial.println(msg, base);
    }
    
    void log(double msg, int base) {
      if (ENABLED) Serial.println(msg, base);
    }
    
    void log(const Printable& msg) {
      if (ENABLED) Serial.println(msg);
    }
};

static DebugLog<ENABLE_DEBUG> debug;

#endif
