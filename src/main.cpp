#include <Arduino.h>
#include <WindVane.h>
#include <Hardware/ESP32/ADC.h>
#include <Storage/FileCalibrationStorage.h>
#include <IO/SerialIOHandler.h>
#include <Diagnostics/SerialDiagnostics.h>
#include "Config.h"

ESP32ADC adc(WINDVANE_GPIO_PIN);
FileCalibrationStorage storage("calibration.dat");
SerialIOHandler io;
SerialDiagnostics diag;
WindVane windVane(&adc, WindVaneType::REED_SWITCH, CalibrationMethod::SPINNING,
                  &storage, &io, &diag);

void setup() {
  Serial.begin(115200);
  windVane.startCalibration();
}

void loop() {}
