#include "EEPROMCalibrationStorage.h"
#include <Platform/IPlatform.h>
#include <PlatformFactory.h>

EEPROMCalibrationStorage::EEPROMCalibrationStorage(IPlatform& platform,
                                                    size_t startAddress,
                                                    size_t eepromSize)
    : _startAddress(startAddress), _eepromSize(eepromSize),
      _slotCount(4), _slotSize(eepromSize / 4), _platform(platform) {}

StorageResult EEPROMCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
    if (!platform_factory::has_eeprom()) {
        (void)clusters;
        (void)version;
        return {StorageStatus::IoError, "no eeprom"};
    }
    int latest = findLatestSlot();
    int slot = latest < 0 ? 0 : (latest + 1) % _slotCount;
    size_t addr = slotAddr(slot);
    platform_factory::eeprom_begin(_eepromSize);
    _schemaVersion = version;
    CalibrationStorageHeader hdr{};
    hdr.version = static_cast<uint16_t>(version);
    hdr.timestamp = platform::toEmbedded(_platform.millis());
    _lastTimestamp = hdr.timestamp;
    hdr.count = static_cast<uint16_t>(clusters.size());
    hdr.crc = crc32(clusters);
    platform_factory::eeprom_write_bytes(addr, &hdr, sizeof(CalibrationStorageHeader));
    addr += sizeof(CalibrationStorageHeader);
    for (const auto& c : clusters) {
        platform_factory::eeprom_write_bytes(addr, &c, sizeof(ClusterData));
        addr += sizeof(ClusterData);
    }
    platform_factory::eeprom_commit();
    platform_factory::eeprom_end();
    return {};
}

StorageResult EEPROMCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
    if (!platform_factory::has_eeprom()) {
        (void)clusters;
        (void)version;
        return {StorageStatus::IoError, "no eeprom"};
    }
    int slot = findLatestSlot();
    if (slot < 0)
        return {StorageStatus::NotFound, "slot"};
    size_t addr = slotAddr(slot);
    platform_factory::eeprom_begin(_eepromSize);
    CalibrationStorageHeader hdr{};
    platform_factory::eeprom_read_bytes(addr, &hdr, sizeof(CalibrationStorageHeader));
    addr += sizeof(CalibrationStorageHeader);
    version = hdr.version;
    _schemaVersion = hdr.version;
    _lastTimestamp = hdr.timestamp;
    uint16_t count = hdr.count;
    if (count == 0 || count > 64) {
        platform_factory::eeprom_end();
        return {StorageStatus::InvalidFormat, "count"};
    }
    clusters.resize(count);
    for (uint16_t i = 0; i < count; ++i) {
        ClusterData c{};
        platform_factory::eeprom_read_bytes(addr, &c, sizeof(ClusterData));
        addr += sizeof(ClusterData);
        clusters[i] = c;
    }
    platform_factory::eeprom_end();
    uint32_t crc = crc32(clusters);
    if (crc != hdr.crc) {
        return {StorageStatus::CorruptData, "crc"};
    }
    return {};
}

StorageResult EEPROMCalibrationStorage::clear() {
    CalibrationStorageHeader hdr{};
    if (!platform_factory::has_eeprom()) {
        (void)hdr;
        return {StorageStatus::IoError, "no eeprom"};
    }
    platform_factory::eeprom_begin(_eepromSize);
    for (int i = 0; i < _slotCount; ++i) {
        size_t addr = slotAddr(i);
        platform_factory::eeprom_write_bytes(addr, &hdr, sizeof(CalibrationStorageHeader));
    }
    platform_factory::eeprom_commit();
    platform_factory::eeprom_end();
    return {};
}

StorageResult EEPROMCalibrationStorage::writeBlob(const std::vector<unsigned char>& data) {
    if (!platform_factory::has_eeprom()) {
        (void)data;
        return {StorageStatus::IoError, "no eeprom"};
    }
    if (data.size() + _startAddress > _eepromSize)
        return {StorageStatus::InvalidFormat, "size"};
    platform_factory::eeprom_begin(_eepromSize);
    for (size_t i = 0; i < data.size(); ++i)
        platform_factory::eeprom_write_byte(_startAddress + i, data[i]);
    platform_factory::eeprom_commit();
    platform_factory::eeprom_end();
    return {};
}

StorageResult EEPROMCalibrationStorage::readBlob(std::vector<unsigned char>& data) {
    if (!platform_factory::has_eeprom()) {
        (void)data;
        return {StorageStatus::IoError, "no eeprom"};
    }
    platform_factory::eeprom_begin(_eepromSize);
    data.resize(_eepromSize - _startAddress);
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = platform_factory::eeprom_read_byte(_startAddress + i);
    platform_factory::eeprom_end();
    return {};
}

int EEPROMCalibrationStorage::findLatestSlot() const {
    if (!platform_factory::has_eeprom()) {
        return -1;
    }
    platform_factory::eeprom_begin(_eepromSize);
    int latest = -1;
    uint32_t latestTs = 0;
    for (int i = 0; i < _slotCount; ++i) {
        size_t addr = slotAddr(i);
        CalibrationStorageHeader hdr{};
        platform_factory::eeprom_read_bytes(addr, &hdr, sizeof(CalibrationStorageHeader));
        if (hdr.version != 0 && hdr.count <= 64) {
            if (hdr.timestamp >= latestTs) {
                latestTs = hdr.timestamp;
                latest = i;
            }
        }
    }
    platform_factory::eeprom_end();
    return latest;
}
