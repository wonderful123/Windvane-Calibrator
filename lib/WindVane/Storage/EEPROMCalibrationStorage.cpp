#include "EEPROMCalibrationStorage.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include <Platform/Platform.h>

EEPROMCalibrationStorage::EEPROMCalibrationStorage(IPlatform& platform,
                                                   size_t startAddress,
                                                   size_t eepromSize)
    : _startAddress(startAddress), _eepromSize(eepromSize), _platform(platform) {}

void EEPROMCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
#ifdef ARDUINO
    size_t addr = _startAddress;
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
#else
    (void)clusters;
    (void)version;
#endif
}

bool EEPROMCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
#ifdef ARDUINO
    size_t addr = _startAddress;
    EEPROM.begin(_eepromSize);
    CalibrationStorageHeader hdr{};
    EEPROM.get(addr, hdr); addr += sizeof(CalibrationStorageHeader);
    version = hdr.version;
    _schemaVersion = hdr.version;
    _lastTimestamp = hdr.timestamp;
    uint16_t count = hdr.count;
    if (count == 0 || count > 64) {
        EEPROM.end();
        return false;
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
        return false;
    }
    return true;
#else
    (void)clusters;
    (void)version;
    return false;
#endif
}

void EEPROMCalibrationStorage::clear() {
#ifdef ARDUINO
    size_t addr = _startAddress;
    EEPROM.begin(_eepromSize);
    CalibrationStorageHeader hdr{};
    EEPROM.put(addr, hdr); addr += sizeof(CalibrationStorageHeader);
    EEPROM.commit();
    EEPROM.end();
    _lastTimestamp = 0;
    _schemaVersion = 0;
#else
    /* no-op */
#endif
}

bool EEPROMCalibrationStorage::writeBlob(const std::vector<unsigned char>& data) {
#ifdef ARDUINO
    if (data.size() + _startAddress > _eepromSize) return false;
    EEPROM.begin(_eepromSize);
    for (size_t i = 0; i < data.size(); ++i)
        EEPROM.write(_startAddress + i, data[i]);
    EEPROM.commit();
    EEPROM.end();
    return true;
#else
    (void)data;
    return false;
#endif
}

bool EEPROMCalibrationStorage::readBlob(std::vector<unsigned char>& data) {
#ifdef ARDUINO
    EEPROM.begin(_eepromSize);
    data.resize(_eepromSize - _startAddress);
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = EEPROM.read(_startAddress + i);
    EEPROM.end();
    return true;
#else
    (void)data;
    return false;
#endif
}
