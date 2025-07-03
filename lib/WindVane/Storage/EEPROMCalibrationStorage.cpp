#include "EEPROMCalibrationStorage.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "../../Platform/Platform.h"

EEPROMCalibrationStorage::EEPROMCalibrationStorage(size_t startAddress,
                                                   size_t eepromSize)
    : _startAddress(startAddress), _eepromSize(eepromSize) {}

void EEPROMCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
#ifdef ARDUINO
    size_t addr = _startAddress;
    EEPROM.begin(_eepromSize);
    EEPROM.put(addr, version); addr += sizeof(int);
    uint32_t timestamp = platformMillis();
    _lastTimestamp = timestamp;
    EEPROM.put(addr, timestamp); addr += sizeof(uint32_t);
    uint16_t count = static_cast<uint16_t>(clusters.size());
    EEPROM.put(addr, count); addr += sizeof(uint16_t);
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
    EEPROM.get(addr, version); addr += sizeof(int);
    uint32_t timestamp = 0;
    EEPROM.get(addr, timestamp); addr += sizeof(uint32_t);
    _lastTimestamp = timestamp;
    uint16_t count = 0;
    EEPROM.get(addr, count); addr += sizeof(uint16_t);
    if (count == 0 || count > 64) {
        EEPROM.end();
        return false;
    }
    clusters.clear();
    for (uint16_t i = 0; i < count; ++i) {
        ClusterData c{};
        EEPROM.get(addr, c); addr += sizeof(ClusterData);
        clusters.push_back(c);
    }
    EEPROM.end();
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
    int version = 0;
    EEPROM.put(addr, version); addr += sizeof(int);
    uint32_t ts = 0;
    EEPROM.put(addr, ts); addr += sizeof(uint32_t);
    uint16_t count = 0;
    EEPROM.put(addr, count); addr += sizeof(uint16_t);
    EEPROM.commit();
    EEPROM.end();
    _lastTimestamp = 0;
#else
    /* no-op */
#endif
}
