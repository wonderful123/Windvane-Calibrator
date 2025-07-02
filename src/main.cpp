#include <Arduino.h>
#include <WindVane.h>
#include <Hardware/ESP32/ADC.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <IO/SerialIOHandler.h>
#include <Diagnostics/BufferedDiagnostics.h>
#include <Menu/ArduinoMenu.h>
#include "Config.h"

#include <memory>

std::unique_ptr<ESP32ADC> adc;
std::unique_ptr<EEPROMCalibrationStorage> storage;
std::unique_ptr<SerialIOHandler> io;
std::unique_ptr<BufferedDiagnostics> diag;
std::unique_ptr<WindVane> windVane;
std::unique_ptr<ArduinoMenu> menu;

void setup() {
  Serial.begin(115200);
  adc = std::make_unique<ESP32ADC>(WINDVANE_GPIO_PIN);
  storage = std::make_unique<EEPROMCalibrationStorage>(0);
  io = std::make_unique<SerialIOHandler>();
  diag = std::make_unique<BufferedDiagnostics>();
  windVane = std::make_unique<WindVane>(adc.get(), WindVaneType::REED_SWITCH,
                                        CalibrationMethod::SPINNING,
                                        storage.get(), io.get(), diag.get());
  menu = std::make_unique<ArduinoMenu>(windVane.get(), io.get(), diag.get());
  menu->begin();
}

void loop() {
  if (menu)
    menu->update();
}
