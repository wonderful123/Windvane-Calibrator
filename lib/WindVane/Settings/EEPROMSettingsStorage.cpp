#include "EEPROMSettingsStorage.h"

EEPROMSettingsStorage::EEPROMSettingsStorage(size_t start)
    : _start(start) {}

void EEPROMSettingsStorage::save(const SettingsData& data) {
#ifdef ARDUINO
    EEPROM.begin(512);
    size_t addr = _start;
    EEPROM.put(addr, data.spin); addr += sizeof(SpinningConfig);
    EEPROM.commit();
    EEPROM.end();
#else
    (void)data;
#endif
}

bool EEPROMSettingsStorage::load(SettingsData& data) {
#ifdef ARDUINO
    EEPROM.begin(512);
    size_t addr = _start;
    EEPROM.get(addr, data.spin); addr += sizeof(SpinningConfig);
    EEPROM.end();
    return true;
#else
    (void)data;
    return false;
#endif
}
