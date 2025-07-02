#include "ADC.h"

#ifdef UNIT_TEST
  #include "ArduinoFake.h"
#else
  #include "Arduino.h"
#endif

float ESP32ADC::read() { return analogRead(_pin); }