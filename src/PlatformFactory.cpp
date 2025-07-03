#include "PlatformFactory.h"
#ifdef ARDUINO
#include <Drivers/ESP32/ADC.h>
#include <Storage/EEPROMCalibrationStorage.h>
#include <Storage/Settings/EEPROMSettingsStorage.h>
#include <UI/SerialIOHandler.h>
#include <UI/SerialOutput.h>
#include <Arduino.h>
#else
#include "host/NullADC.h"
#include <Storage/FileCalibrationStorage.h>
#include <Storage/Settings/FileSettingsStorage.h>
#include <UI/ConsoleIOHandler.h>
#include <UI/ConsoleOutput.h>
#include <chrono>
#endif
#include <cstring>
#include <WindVaneMenu/MenuPresenter.h>

namespace {

#ifdef ARDUINO
class ArduinoPlatform : public IPlatform {
public:
    platform::TimeMs millis() const override { return platform::TimeMs{::millis()}; }
    void renderStatusLine(MenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) const override {
        presenter.renderStatusLine(st, statusStr, msg, level, supportsColor());
    }
    bool supportsColor() const override { return false; }
};
#else
class HostPlatform : public IPlatform {
public:
    platform::TimeMs millis() const override {
        using namespace std::chrono;
        static auto start = steady_clock::now();
        return platform::TimeMs{static_cast<platform::TimeMs::rep>(duration_cast<milliseconds>(steady_clock::now() - start).count())};
    }
    void renderStatusLine(MenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) const override {
        presenter.renderStatusLine(st, statusStr, msg, level, supportsColor());
    }
    bool supportsColor() const override { return true; }
};
#endif

} // namespace

namespace platform {
TimeMs now() {
#ifdef ARDUINO
    return TimeMs{::millis()};
#else
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return TimeMs{static_cast<TimeMs::rep>(duration_cast<milliseconds>(steady_clock::now() - start).count())};
#endif
}
} // namespace platform

namespace platform_factory {

std::unique_ptr<IPlatform> makePlatform() {
#ifdef ARDUINO
    return std::make_unique<ArduinoPlatform>();
#else
    return std::make_unique<HostPlatform>();
#endif
}

std::unique_ptr<IUserIO> makeIO() {
#ifdef ARDUINO
    return std::make_unique<SerialIOHandler>();
#else
    return std::make_unique<ConsoleIOHandler>();
#endif
}

std::unique_ptr<IOutput> makeOutput() {
#ifdef ARDUINO
    return std::make_unique<SerialOutput>();
#else
    return std::make_unique<ConsoleOutput>();
#endif
}

void beginPlatformIO(unsigned long baud) {
#ifdef ARDUINO
    Serial.begin(baud);
#else
    (void)baud;
#endif
}

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

void eeprom_begin(size_t size) {
#ifdef ARDUINO
    EEPROM.begin(size);
#else
    (void)size;
#endif
}

void eeprom_commit() {
#ifdef ARDUINO
    EEPROM.commit();
#endif
}

void eeprom_end() {
#ifdef ARDUINO
    EEPROM.end();
#endif
}

void eeprom_write_bytes(size_t addr, const void* data, size_t len) {
#ifdef ARDUINO
    const uint8_t* p = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < len; ++i) EEPROM.write(addr + i, p[i]);
#else
    (void)addr; (void)data; (void)len;
#endif
}

void eeprom_read_bytes(size_t addr, void* data, size_t len) {
#ifdef ARDUINO
    uint8_t* p = static_cast<uint8_t*>(data);
    for (size_t i = 0; i < len; ++i) p[i] = EEPROM.read(addr + i);
#else
    (void)addr; (void)data; (void)len;
#endif
}

uint8_t eeprom_read_byte(size_t addr) {
#ifdef ARDUINO
    return EEPROM.read(addr);
#else
    (void)addr; return 0;
#endif
}

void eeprom_write_byte(size_t addr, uint8_t value) {
#ifdef ARDUINO
    EEPROM.write(addr, value);
#else
    (void)addr; (void)value;
#endif
}

bool has_eeprom() {
#ifdef ARDUINO
    return true;
#else
    return false;
#endif
}

} // namespace platform_factory
