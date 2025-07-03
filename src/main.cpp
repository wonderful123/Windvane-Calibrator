#include "App.h"
#include "Config.h"

#include <Platform/Platform.h>
#include <UI/IOFactory.h>
#include <Storage/Settings/SettingsManager.h>
#include "PlatformFactory.h"

DeviceConfig deviceCfg = defaultDeviceConfig();

Platform platform;
auto adc = platform_factory::makeADC(deviceCfg);
auto calibStorage = platform_factory::makeCalibrationStorage(platform, deviceCfg);
auto settingsStorage = platform_factory::makeSettingsStorage(deviceCfg);

auto ioPtr = ui::makeDefaultIO();
auto outPtr = ui::makeDefaultOutput();
PlatformIOHandler& io = *ioPtr;
PlatformOutput& out = *outPtr;
PlatformDiagnostics diag;
PlatformDiagnosticsSink sink(outPtr.get());
diag.addSink(&sink);
SettingsManager settingsMgr(*settingsStorage, diag);

WindVaneConfig vaneCfg{*adc, WindVaneType::REED_SWITCH,
                       CalibrationMethod::SPINNING, calibStorage.get(), io, diag,
                       {}};
WindVane vane(vaneCfg);

App app(deviceCfg, vane, io, diag, out, *calibStorage, settingsMgr, platform);

void setup() {
  ui::beginPlatformIO(deviceCfg.serialBaud);
  app.begin();
}

void loop() { app.loop(); }
