#include <Arduino.h>
#include <WindVane.h>
#include <Hardware/ESP32/ADC.h>
#include <Storage/FileCalibrationStorage.h>
#include "Config.h"

ESP32ADC adc(WINDVANE_GPIO_PIN);
FileCalibrationStorage storage("calibration.dat");
WindVane windVane(&adc, WindVaneType::REED_SWITCH, CalibrationMethod::SPINNING,
                  &storage);

void setup() {
  Serial.begin(115200);
  windVane.startCalibration();
}

void loop() {}
