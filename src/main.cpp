#include "App.h"
#include "Config.h"

#include <Diagnostics/DiagnosticsBus.h>
#include <Diagnostics/BasicDiagnostics.h>
#include <UI/IOFactory.h>
#include <PlatformFactory.h>
#include <Storage/Settings/SettingsManager.h>

DeviceConfig deviceCfg = defaultDeviceConfig();

auto platformPtr = platform_factory::makePlatform();
auto adc = platform_factory::makeADC(deviceCfg);
auto calibStorage = platform_factory::makeCalibrationStorage(*platformPtr, deviceCfg);
auto settingsStorage = platform_factory::makeSettingsStorage(deviceCfg);

auto ioPtr = ui::makeDefaultIO();
auto outPtr = ui::makeDefaultOutput();
IUserIO& io = *ioPtr;
IOutput& out = *outPtr;
DiagnosticsBus diag;
BasicDiagnostics sink(outPtr.get());
SettingsManager settingsMgr(*settingsStorage, diag);

WindVaneConfig vaneCfg{*adc, WindVaneType::REED_SWITCH,
                       CalibrationMethod::SPINNING, calibStorage.get(), io, diag,
                       {}};
WindVane vane(vaneCfg);

App app(deviceCfg, vane, io, diag, out, *calibStorage, settingsMgr, *platformPtr);

void setup() {
  ui::beginPlatformIO(deviceCfg.serialBaud);
  diag.addSink(&sink);
  app.begin();
}

void loop() { app.loop(); }
