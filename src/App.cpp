#include "App.h"

#include <Diagnostics/SerialDiagnostics.h>
#include <IO/SerialOutput.h>
#include <IO/ConsoleOutput.h>
#include <IO/SerialIOHandler.h>
#include <Settings/EEPROMSettingsStorage.h>
#include <Settings/FileSettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Settings/SettingsManager.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <Storage/FileCalibrationStorage.h>

App::App(const DeviceConfig& config)
    : cfg(config), vane(cfg.windVanePin, cfg.serialBaud), menu(nullptr) {}

void App::begin() {
  static SerialIOHandler io;
#ifdef ARDUINO
  static SerialOutput out;
#else
  static ConsoleOutput out;
#endif
  static SerialDiagnostics diag;

#ifdef ARDUINO
  static EEPROMCalibrationStorage storage(cfg.calibrationAddress,
                                          cfg.eepromSize);
  static EEPROMSettingsStorage settingsStorage(cfg.settingsAddress,
                                               cfg.eepromSize);
#else
  static FileCalibrationStorage storage("calib.dat");
  static FileSettingsStorage settingsStorage(cfg.settingsFile);
#endif
  static SettingsData settings;
  static SettingsManager settingsMgr(&settingsStorage, &settings, &diag);
  settingsMgr.load();
  settingsMgr.apply(vane);

  WindVaneMenuConfig menuCfg{vane, io, diag, nullptr, out, storage, settingsMgr, io};

  menu = std::make_unique<WindVaneMenu>(menuCfg);
  menu->begin();
}

void App::loop() {
  if (menu) menu->update();
}
