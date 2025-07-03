#include "App.h"

#include <Diagnostics/SerialDiagnostics.h>
#include <IO/SerialIOHandler.h>
#include <Settings/EEPROMSettingsStorage.h>
#include <Settings/FileSettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <Storage/FileCalibrationStorage.h>

App::App(const DeviceConfig& config)
    : cfg(config), vane(cfg.windVanePin, cfg.serialBaud), menu(nullptr) {}

void App::begin() {
  static SerialIOHandler io;
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

  WindVaneMenuConfig menuCfg;
  menuCfg.vane = &vane;
  menuCfg.io = &io;
  menuCfg.diag = &diag;
  menuCfg.bufferedDiag = nullptr;
  menuCfg.out = nullptr;
  menuCfg.storage = &storage;
  menuCfg.settingsStorage = &settingsStorage;
  menuCfg.settings = &settings;
  menuCfg.numeric = &io;

  menu = new WindVaneMenu(menuCfg);
  menu->begin();
}

void App::loop() {
  if (menu) menu->update();
}
