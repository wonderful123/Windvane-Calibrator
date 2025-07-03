#pragma once
#include <WindVane.h>
#include <memory>
#include <WindVaneMenu.h>
#include <UI/IIO.h>
#include <Diagnostics/IDiagnostics.h>
#include <Storage/ICalibrationStorage.h>
#include <Storage/Settings/SettingsManager.h>
#include <Platform/IPlatform.h>

#include "Config.h"

class App {
 public:
  App(const DeviceConfig& config, WindVane& vane, IUserIO& io,
      IDiagnostics& diag, IOutput& out, ICalibrationStorage& storage,
      SettingsManager& settingsMgr, IPlatform& platform);

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
  IPlatform& platform;
  std::unique_ptr<WindVaneMenu> menu;
};
