#include "EEPROMSettingsStorage.h"

EEPROMSettingsStorage::EEPROMSettingsStorage(size_t start, size_t eepromSize)
    : _start(start), _size(eepromSize) {}

StorageResult EEPROMSettingsStorage::save(const SettingsData& data) {
#ifdef ARDUINO
    EEPROM.begin(_size);
    size_t addr = _start;
    SettingsStorageHeader hdr{};
    hdr.version = 1;
    hdr.crc = SettingsStorageBase::crc32(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
    EEPROM.put(addr, hdr); addr += sizeof(SettingsStorageHeader);
    EEPROM.put(addr, data); addr += sizeof(SettingsData);
    EEPROM.commit();
    EEPROM.end();
    _schemaVersion = hdr.version;
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
    SettingsStorageHeader hdr{};
    EEPROM.get(addr, hdr); addr += sizeof(SettingsStorageHeader);
    EEPROM.get(addr, data); addr += sizeof(SettingsData);
    EEPROM.end();
    uint32_t crc = SettingsStorageBase::crc32(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
    if (crc != hdr.crc)
        return {StorageStatus::CorruptData, "crc"};
    _schemaVersion = hdr.version;
    return {};
#else
    (void)data;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}
