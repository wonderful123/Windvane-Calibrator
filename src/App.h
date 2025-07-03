#pragma once
#include <WindVane.h>
#include <memory>
#include <WindVaneMenu.h>
#include <IO/IUserIO.h>
#include <Diagnostics/IDiagnostics.h>
#include <IO/IOutput.h>
#include <Storage/ICalibrationStorage.h>
#include <Settings/SettingsManager.h>

#include "Config.h"

class App {
 public:
  App(const DeviceConfig& config, WindVane& vane, IUserIO& io,
      IDiagnostics& diag, IOutput& out, ICalibrationStorage& storage,
      SettingsManager& settingsMgr);

  void begin();
  void loop();

 private:
  const DeviceConfig& cfg;
  WindVane& vane;
  IUserIO& io;
  IDiagnostics& diag;
  IOutput& out;
  ICalibrationStorage& storage;
  SettingsManager& settingsMgr;
  std::unique_ptr<WindVaneMenu> menu;
};
