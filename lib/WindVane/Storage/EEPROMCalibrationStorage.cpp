#include "EEPROMCalibrationStorage.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif

EEPROMCalibrationStorage::EEPROMCalibrationStorage(size_t startAddress)
    : _startAddress(startAddress) {}

void EEPROMCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
#ifdef ARDUINO
    size_t addr = _startAddress;
    EEPROM.begin(clusters.size() * sizeof(float));
    for (const auto& c : clusters) {
        EEPROM.put(addr, c.mean);
        addr += sizeof(float);
    }
    EEPROM.commit();
#else
    (void)clusters;
    (void)version;
#endif
}

bool EEPROMCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
#ifdef ARDUINO
    (void)version;
    clusters.clear();
    size_t addr = _startAddress;
    EEPROM.begin(512);
    // Implementation omitted for brevity
    EEPROM.end();
    return false;
#else
    (void)clusters;
    (void)version;
    return false;
#endif
}
