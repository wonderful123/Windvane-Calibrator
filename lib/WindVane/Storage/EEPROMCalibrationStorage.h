#pragma once
#include "ICalibrationStorage.h"
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMCalibrationStorage : public ICalibrationStorage {
public:
    EEPROMCalibrationStorage(size_t startAddress = 0);
    void save(const std::vector<ClusterData>& clusters, int version) override;
    bool load(std::vector<ClusterData>& clusters, int &version) override;
    uint32_t lastTimestamp() const { return _lastTimestamp; }

private:
    size_t _startAddress;
    uint32_t _lastTimestamp{0};
};
