#include "EEPROMCalibrationStorage.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include <Platform/Platform.h>

EEPROMCalibrationStorage::EEPROMCalibrationStorage(IPlatform& platform,
                                                    size_t startAddress,
                                                    size_t eepromSize)
    : _startAddress(startAddress), _eepromSize(eepromSize),
      _slotCount(4), _slotSize(eepromSize / 4), _platform(platform) {}

StorageResult EEPROMCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
#ifdef ARDUINO
    int latest = findLatestSlot();
    int slot = latest < 0 ? 0 : (latest + 1) % _slotCount;
    size_t addr = slotAddr(slot);
    EEPROM.begin(_eepromSize);
    _schemaVersion = version;
    CalibrationStorageHeader hdr{};
    hdr.version = static_cast<uint16_t>(version);
    hdr.timestamp = platform::toEmbedded(_platform.millis());
    _lastTimestamp = hdr.timestamp;
    hdr.count = static_cast<uint16_t>(clusters.size());
    hdr.crc = crc32(clusters);
    EEPROM.put(addr, hdr); addr += sizeof(CalibrationStorageHeader);
    for (const auto& c : clusters) {
        EEPROM.put(addr, c); addr += sizeof(ClusterData);
    }
    EEPROM.commit();
    return {};
#else
    (void)clusters;
    (void)version;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}

StorageResult EEPROMCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
#ifdef ARDUINO
    int slot = findLatestSlot();
    if (slot < 0)
        return {StorageStatus::NotFound, "slot"};
    size_t addr = slotAddr(slot);
    EEPROM.begin(_eepromSize);
    CalibrationStorageHeader hdr{};
    EEPROM.get(addr, hdr); addr += sizeof(CalibrationStorageHeader);
    version = hdr.version;
    _schemaVersion = hdr.version;
    _lastTimestamp = hdr.timestamp;
    uint16_t count = hdr.count;
    if (count == 0 || count > 64) {
        EEPROM.end();
        return {StorageStatus::InvalidFormat, "count"};
    }
    clusters.resize(count);
    for (uint16_t i = 0; i < count; ++i) {
        ClusterData c{};
        EEPROM.get(addr, c); addr += sizeof(ClusterData);
        clusters[i] = c;
    }
    EEPROM.end();
    uint32_t crc = crc32(clusters);
    if (crc != hdr.crc) {
        return {StorageStatus::CorruptData, "crc"};
    }
    return {};
#else
    (void)clusters;
    (void)version;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}

void EEPROMCalibrationStorage::clear() {
    CalibrationStorageHeader hdr{};
#ifdef ARDUINO
    EEPROM.begin(_eepromSize);
    for (int i = 0; i < _slotCount; ++i) {
        size_t addr = slotAddr(i);
        EEPROM.put(addr, hdr);
    }
    EEPROM.commit();
    EEPROM.end();
#else
    (void)hdr;
#endif
}

StorageResult EEPROMCalibrationStorage::writeBlob(const std::vector<unsigned char>& data) {
#ifdef ARDUINO
    if (data.size() + _startAddress > _eepromSize)
        return {StorageStatus::InvalidFormat, "size"};
    EEPROM.begin(_eepromSize);
    for (size_t i = 0; i < data.size(); ++i)
        EEPROM.write(_startAddress + i, data[i]);
    EEPROM.commit();
    EEPROM.end();
    return {};
#else
    (void)data;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}

StorageResult EEPROMCalibrationStorage::readBlob(std::vector<unsigned char>& data) {
#ifdef ARDUINO
    EEPROM.begin(_eepromSize);
    data.resize(_eepromSize - _startAddress);
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = EEPROM.read(_startAddress + i);
    EEPROM.end();
    return {};
#else
    (void)data;
    return {StorageStatus::IoError, "no eeprom"};
#endif
}

int EEPROMCalibrationStorage::findLatestSlot() const {
#ifdef ARDUINO
    EEPROM.begin(_eepromSize);
    int latest = -1;
    uint32_t latestTs = 0;
    for (int i = 0; i < _slotCount; ++i) {
        size_t addr = slotAddr(i);
        CalibrationStorageHeader hdr{};
        EEPROM.get(addr, hdr);
        if (hdr.version != 0 && hdr.count <= 64) {
            if (hdr.timestamp >= latestTs) {
                latestTs = hdr.timestamp;
                latest = i;
            }
        }
    }
    EEPROM.end();
    return latest;
#else
    return -1;
#endif
}
