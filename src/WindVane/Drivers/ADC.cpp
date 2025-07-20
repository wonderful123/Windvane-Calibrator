#include "ADC.h"

#ifdef UNIT_TEST
  #include "ArduinoFake.h"
#else
  #include "Arduino.h"
#endif

float ESP32ADC::read() const { 
    // ESP32 ADC returns 0-4095, normalize to 0.0-1.0
    int rawValue = analogRead(_pin);
    
    // Clamp to valid range (defensive programming)
    if (rawValue < 0) rawValue = 0;
    if (rawValue > 4095) rawValue = 4095;
    
    return static_cast<float>(rawValue) / 4095.0f; 
}
