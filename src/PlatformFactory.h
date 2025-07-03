#pragma once
#include <memory>
#include <IADC.h>
#include <Storage/ICalibrationStorage.h>
#include <Storage/Settings/ISettingsStorage.h>
#include <Platform/IPlatform.h>
#include <UI/IIO.h>
#include "Config.h"

namespace platform_factory {
std::unique_ptr<IPlatform> makePlatform();
std::unique_ptr<IADC> makeADC(const DeviceConfig& cfg);
std::unique_ptr<ICalibrationStorage> makeCalibrationStorage(IPlatform& platform, const DeviceConfig& cfg);
std::unique_ptr<ISettingsStorage> makeSettingsStorage(const DeviceConfig& cfg);
std::unique_ptr<IUserIO> makeIO();
std::unique_ptr<IOutput> makeOutput();
void beginPlatformIO(unsigned long baud);

void eeprom_begin(size_t size);
void eeprom_commit();
void eeprom_end();
void eeprom_write_bytes(size_t addr, const void* data, size_t len);
void eeprom_read_bytes(size_t addr, void* data, size_t len);
uint8_t eeprom_read_byte(size_t addr);
void eeprom_write_byte(size_t addr, uint8_t value);
bool has_eeprom();
} // namespace platform_factory

namespace platform {
TimeMs now();
}
