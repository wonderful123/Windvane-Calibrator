#include "App.h"
#include "Config.h"

#include <Platform/Platform.h>
#include <Storage/Settings/EEPROMSettingsStorage.h>
#include <Storage/Settings/FileSettingsStorage.h>
#include <Storage/Settings/SettingsData.h>
#include <Storage/Settings/SettingsManager.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <Storage/FileCalibrationStorage.h>
#ifdef ARDUINO
#include <Drivers/ESP32/ADC.h>
#else
#include "host/NullADC.h"
#endif

DeviceConfig deviceCfg = defaultDeviceConfig();

#ifdef ARDUINO
ESP32ADC adc(deviceCfg.windVanePin);
Platform platform;
EEPROMCalibrationStorage calibStorage(platform, deviceCfg.calibrationAddress,
                                      deviceCfg.eepromSize);
EEPROMSettingsStorage settingsStorage(deviceCfg.settingsAddress,
                                      deviceCfg.eepromSize);
#else
NullADC adc;
Platform platform;
FileCalibrationStorage calibStorage("calib.dat");
FileSettingsStorage settingsStorage(deviceCfg.settingsFile);
#endif

PlatformIOHandler io;
PlatformOutput out;
PlatformDiagnostics diag;
SettingsData settings;
SettingsManager settingsMgr(&settingsStorage, &settings, &diag);

WindVaneConfig vaneCfg{&adc, WindVaneType::REED_SWITCH,
                       CalibrationMethod::SPINNING, &calibStorage, &io, &diag,
                       {}};
WindVane vane(vaneCfg);

App app(deviceCfg, vane, io, diag, out, calibStorage, settingsMgr, platform);

void setup() {
#ifdef ARDUINO
  Serial.begin(deviceCfg.serialBaud);
#endif
  app.begin();
}

void loop() { app.loop(); }
