#include "EEPROMSettingsStorage.h"
#include <PlatformFactory.h>

EEPROMSettingsStorage::EEPROMSettingsStorage(size_t start, size_t eepromSize)
    : _start(start), _size(eepromSize) {}

StorageResult EEPROMSettingsStorage::save(const SettingsData& data) {
    if (!platform_factory::has_eeprom()) {
        (void)data;
        return {StorageStatus::IoError, "no eeprom"};
    }
    platform_factory::eeprom_begin(_size);
    size_t addr = _start;
    SettingsStorageHeader hdr{};
    hdr.version = 1;
    hdr.crc = SettingsStorageBase::crc32(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
    platform_factory::eeprom_write_bytes(addr, &hdr, sizeof(SettingsStorageHeader));
    addr += sizeof(SettingsStorageHeader);
    platform_factory::eeprom_write_bytes(addr, &data, sizeof(SettingsData));
    addr += sizeof(SettingsData);
    platform_factory::eeprom_commit();
    platform_factory::eeprom_end();
    _schemaVersion = hdr.version;
    return {};
}

StorageResult EEPROMSettingsStorage::load(SettingsData& data) {
    if (!platform_factory::has_eeprom()) {
        (void)data;
        return {StorageStatus::IoError, "no eeprom"};
    }
    platform_factory::eeprom_begin(_size);
    size_t addr = _start;
    SettingsStorageHeader hdr{};
    platform_factory::eeprom_read_bytes(addr, &hdr, sizeof(SettingsStorageHeader));
    addr += sizeof(SettingsStorageHeader);
    platform_factory::eeprom_read_bytes(addr, &data, sizeof(SettingsData));
    addr += sizeof(SettingsData);
    platform_factory::eeprom_end();
    uint32_t crc = SettingsStorageBase::crc32(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
    if (crc != hdr.crc)
        return {StorageStatus::CorruptData, "crc"};
    _schemaVersion = hdr.version;
    return {};
}
