#include "App.h"
#include <optional>

App::App(const DeviceConfig& config, WindVane& v, IUserIO& ioRef,
         IDiagnostics& diagRef, IOutput& outRef, ICalibrationStorage& storageRef,
         SettingsManager& settingsMgrRef, IPlatform& platformRef)
    : cfg(config),
      vane(v),
      io(ioRef),
      diag(diagRef),
      out(outRef),
      storage(storageRef),
      settingsMgr(settingsMgrRef),
      platform(platformRef),
      menu(nullptr) {}

void App::begin() {
  settingsMgr.load();
  settingsMgr.apply(vane);

  WindVaneMenuConfig menuCfg{vane, io, diag, std::nullopt, out, storage,
                             settingsMgr, platform};
  menu = std::make_unique<WindVaneMenu>(menuCfg);
  menu->begin();
}

void App::loop() {
  if (menu) menu->update();
}
