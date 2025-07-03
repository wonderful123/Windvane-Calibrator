#include "EEPROMSettingsStorage.h"

EEPROMSettingsStorage::EEPROMSettingsStorage(size_t start, size_t eepromSize)
    : _start(start), _size(eepromSize) {}

StorageResult EEPROMSettingsStorage::save(const SettingsData& data) {
#ifdef ARDUINO
    EEPROM.begin(_size);
    size_t addr = _start;
    EEPROM.put(addr, data.spin); addr += sizeof(SpinningConfig);
    EEPROM.commit();
    EEPROM.end();
    return {};
#else
    (void)data;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}

StorageResult EEPROMSettingsStorage::load(SettingsData& data) {
#ifdef ARDUINO
    EEPROM.begin(_size);
    size_t addr = _start;
    EEPROM.get(addr, data.spin); addr += sizeof(SpinningConfig);
    EEPROM.end();
    return {};
#else
    (void)data;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}
