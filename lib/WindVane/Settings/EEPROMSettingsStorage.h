#pragma once
#include "ISettingsStorage.h"
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMSettingsStorage : public ISettingsStorage {
public:
    explicit EEPROMSettingsStorage(size_t startAddress = 256);
    void save(const SettingsData& data) override;
    bool load(SettingsData& data) override;
private:
    size_t _start;
};
