#include "App.h"
#include "Config.h"

#include <Diagnostics/DiagnosticsBus.h>
#include <Diagnostics/BasicDiagnostics.h>
#include <UI/IOFactory.h>
#include <PlatformFactory.h>
#include <Storage/Settings/SettingsManager.h>

namespace {

struct RuntimeContext {
  DeviceConfig cfg{defaultDeviceConfig()};
  std::unique_ptr<IPlatform> platform{platform_factory::makePlatform()};
  std::unique_ptr<IADC> adc{platform_factory::makeADC(cfg)};
  std::unique_ptr<ICalibrationStorage> calib{platform_factory::makeCalibrationStorage(*platform, cfg)};
  std::unique_ptr<ISettingsStorage> settings{platform_factory::makeSettingsStorage(cfg)};
  std::unique_ptr<IUserIO> io{ui::makeDefaultIO()};
  std::unique_ptr<IOutput> out{ui::makeDefaultOutput()};
  DiagnosticsBus diag{};
  BasicDiagnostics sink{out.get()};
  SettingsManager settingsMgr{*settings, diag};
  WindVane vane{WindVaneConfig{*adc, WindVaneType::REED_SWITCH,
                               CalibrationMethod::SPINNING, calib.get(), *io, diag,
                               {}}};
  App app{cfg, vane, *io, diag, *out, *calib, settingsMgr, *platform};
};

RuntimeContext& ctx() {
  static RuntimeContext instance{};
  return instance;
}

} // namespace

void setup() {
  auto& c = ctx();
  ui::beginPlatformIO(c.cfg.serialBaud);
  c.diag.addSink(&c.sink);
  c.app.begin();
}

void loop() { ctx().app.loop(); }
