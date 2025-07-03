#pragma once
#include "ICalibrationStorage.h"
#include <cstdint>
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMCalibrationStorage : public ICalibrationStorage {
public:
    EEPROMCalibrationStorage(size_t startAddress = 0, size_t eepromSize = 512);
    void save(const std::vector<ClusterData>& clusters, int version) override;
    bool load(std::vector<ClusterData>& clusters, int &version) override;
    uint32_t lastTimestamp() const { return _lastTimestamp; }
    void clear() override;

private:
    size_t _startAddress;
    size_t _eepromSize;
    uint32_t _lastTimestamp{0};
};
