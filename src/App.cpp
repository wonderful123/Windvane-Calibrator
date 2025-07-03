#include "App.h"

App::App(const DeviceConfig& config, WindVane& v, IUserIO& ioRef,
         IDiagnostics& diagRef, IOutput& outRef, ICalibrationStorage& storageRef,
         SettingsManager& settingsMgrRef)
    : cfg(config), vane(v), io(ioRef), diag(diagRef), out(outRef),
      storage(storageRef), settingsMgr(settingsMgrRef), menu(nullptr) {}

void App::begin() {
  settingsMgr.load();
  settingsMgr.apply(vane);

  WindVaneMenuConfig menuCfg{vane, io, diag, nullptr, out, storage, settingsMgr};
  menu = std::make_unique<WindVaneMenu>(menuCfg);
  menu->begin();
}

void App::loop() {
  if (menu) menu->update();
}
