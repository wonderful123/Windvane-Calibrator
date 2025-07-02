#include <Arduino.h>
#include <WindVane.h>
#include <Hardware/ESP32/ADC.h>
#include "Config.h"

ESP32::ADC adc(WINDVANE_GPIO_PIN);
WindVane windVane(&adc, WindVaneType::REED_SWITCH, CalibrationMethod::SPINNING);

void setup() {
  Serial.begin(115200);
}

void loop() {}