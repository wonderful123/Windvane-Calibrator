#include "App.h"
#include "Config.h"

#include <Platform/Platform.h>
#include <Settings/EEPROMSettingsStorage.h>
#include <Settings/FileSettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Settings/SettingsManager.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <Storage/FileCalibrationStorage.h>
#ifdef ARDUINO
#include <Hardware/ESP32/ADC.h>
#else
// Simple stub ADC for host builds
class NullADC : public IADC { public: float read() override { return 0.0f; } };
#endif

DeviceConfig deviceCfg = defaultDeviceConfig();

#ifdef ARDUINO
ESP32ADC adc(deviceCfg.windVanePin);
EEPROMCalibrationStorage calibStorage(deviceCfg.calibrationAddress,
                                      deviceCfg.eepromSize);
EEPROMSettingsStorage settingsStorage(deviceCfg.settingsAddress,
                                      deviceCfg.eepromSize);
#else
NullADC adc;
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

App app(deviceCfg, vane, io, diag, out, calibStorage, settingsMgr);

void setup() {
#ifdef ARDUINO
  Serial.begin(deviceCfg.serialBaud);
#endif
  app.begin();
}

void loop() { app.loop(); }
