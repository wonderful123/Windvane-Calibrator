#pragma once
#include "ICalibrationStorage.h"
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMCalibrationStorage : public ICalibrationStorage {
public:
    EEPROMCalibrationStorage(size_t startAddress = 0);
    void save(const std::vector<ClusterData>& clusters, int version) override;

private:
    size_t _startAddress;
};
