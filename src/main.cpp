#include <Arduino.h>
#include <Menu/ArduinoMenu.h>
#include <WindVane/Diagnostics/BufferedDiagnostics.h>
#include <WindVane/Hardware/ESP32/ADC.h>
#include <WindVane/IO/SerialIOHandler.h>
#include <WindVane/IO/SerialOutput.h>
#include <WindVane/Settings/FileSettingsStorage.h>
#include <WindVane/Settings/SettingsData.h>
#include <WindVane/Storage/EEPROMCalibrationStorage.h>
#include <WindVane/WindVane.h>

#include <memory>

#include "Config.h"

std::unique_ptr<ESP32ADC> adc;
std::unique_ptr<EEPROMCalibrationStorage> storage;
std::unique_ptr<SerialIOHandler> io;
std::unique_ptr<BufferedDiagnostics> diag;
std::unique_ptr<FileSettingsStorage> settingsStore;
std::unique_ptr<SerialOutput> out;
SettingsData settings;
std::unique_ptr<WindVane> windVane;
std::unique_ptr<ArduinoMenu> menu;
DeviceConfig deviceConfig = defaultDeviceConfig();

void setup() {
  Serial.begin(deviceConfig.serialBaud);
  adc = std::make_unique<ESP32ADC>(deviceConfig.windVanePin);
  storage = std::make_unique<EEPROMCalibrationStorage>(
      deviceConfig.calibrationAddress, deviceConfig.eepromSize);
  io = std::make_unique<SerialIOHandler>();
  out = std::make_unique<SerialOutput>();
  diag = std::make_unique<BufferedDiagnostics>();
  settingsStore =
      std::make_unique<FileSettingsStorage>(deviceConfig.settingsFile);
  settingsStore->load(settings);
  WindVaneConfig vaneCfg{adc.get(),
                         WindVaneType::REED_SWITCH,
                         CalibrationMethod::SPINNING,
                         storage.get(),
                         io.get(),
                         diag.get(),
                         settings.spin};
  windVane = std::make_unique<WindVane>(vaneCfg);
  // ArduinoMenuConfig: pass diag.get() as both diag and bufferedDiag
  ArduinoMenuConfig menuCfg{windVane.get(),      io.get(),  diag.get(),
                            diag.get(),          out.get(), storage.get(),
                            settingsStore.get(), &settings};
  menu = std::make_unique<ArduinoMenu>(menuCfg);
  menu->begin();
}

void loop() {
  if (menu) menu->update();
}
