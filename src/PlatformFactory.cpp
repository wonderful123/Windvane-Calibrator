#include "PlatformFactory.h"
#ifdef ARDUINO
#include <Drivers/ESP32/ADC.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <Storage/Settings/EEPROMSettingsStorage.h>
#else
#include "host/NullADC.h"
#include <Storage/FileCalibrationStorage.h>
#include <Storage/Settings/FileSettingsStorage.h>
#endif

namespace platform_factory {

std::unique_ptr<IADC> makeADC(const DeviceConfig& cfg) {
#ifdef ARDUINO
    return std::make_unique<ESP32ADC>(cfg.windVanePin);
#else
    return std::make_unique<NullADC>();
#endif
}

std::unique_ptr<ICalibrationStorage> makeCalibrationStorage(IPlatform& platform, const DeviceConfig& cfg) {
#ifdef ARDUINO
    return std::make_unique<EEPROMCalibrationStorage>(platform, cfg.calibrationAddress, cfg.eepromSize);
#else
    return std::make_unique<FileCalibrationStorage>("calib.dat");
#endif
}

std::unique_ptr<ISettingsStorage> makeSettingsStorage(const DeviceConfig& cfg) {
#ifdef ARDUINO
    return std::make_unique<EEPROMSettingsStorage>(cfg.settingsAddress, cfg.eepromSize);
#else
    return std::make_unique<FileSettingsStorage>(cfg.settingsFile);
#endif
}

} // namespace platform_factory
