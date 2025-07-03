#include "App.h"

#include <Arduino.h>

App::App(const DeviceConfig& deviceCfg)
    : adc(deviceCfg.windVanePin),
      storage(deviceCfg.calibrationAddress, deviceCfg.eepromSize),
      io(),
      out(),
      diag(),
      settingsStore(deviceCfg.settingsFile),
      settings(),
      windVane({&adc, WindVaneType::REED_SWITCH, CalibrationMethod::SPINNING,
                &storage, &io, &diag, settings.spin}),
      menu({
          &windVane, &io, &diag, &diag, &out, &storage, &settingsStore,
          &settings,
          &io  // for numeric input
      }),
      config(deviceCfg) {
  // Nothing else to do here; all dependencies are initialized above.
}

void App::begin() {
  Serial.begin(config.serialBaud);
  settingsStore.load(settings);
  menu.begin();
}

void App::loop() { menu.update(); }
