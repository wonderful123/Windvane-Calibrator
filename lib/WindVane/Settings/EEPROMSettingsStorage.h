#pragma once
#include "ISettingsStorage.h"
#include <cstddef>
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMSettingsStorage : public ISettingsStorage {
public:
    explicit EEPROMSettingsStorage(size_t startAddress = 256, size_t eepromSize = 512);
    void save(const SettingsData& data) override;
    bool load(SettingsData& data) override;
private:
    size_t _start;
    size_t _size;
};
