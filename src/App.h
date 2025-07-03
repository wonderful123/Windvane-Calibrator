#pragma once

#include <Diagnostics/BufferedDiagnostics.h>
#include <Hardware/ESP32/ADC.h>
#include <IO/SerialIOHandler.h>
#include <IO/SerialOutput.h>
#include <ArduinoMenu.h>
#include <Settings/FileSettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <WindVane.h>

#include "Config.h"

class App {
 public:
  App(const DeviceConfig& deviceCfg = defaultDeviceConfig());
  void begin();
  void loop();

 private:
  // Order matters for dependency wiring.
  ESP32ADC adc;
  EEPROMCalibrationStorage storage;
  SerialIOHandler io;
  SerialOutput out;
  BufferedDiagnostics diag;
  FileSettingsStorage settingsStore;
  SettingsData settings;
  WindVane windVane;
  ArduinoMenu menu;

  // Keep a copy of config for possible future dynamic config
  DeviceConfig config;
};
