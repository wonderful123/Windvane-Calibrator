#pragma once
#include "ICalibrationStorage.h"
#include "IBlobStorage.h"
#include <Platform/TimeUtils.h>
#include <Platform/IPlatform.h>
#include <cstdint>
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMCalibrationStorage : public ICalibrationStorage, public IBlobStorage {
public:
    EEPROMCalibrationStorage(IPlatform& platform,
                             size_t startAddress = 0,
                             size_t eepromSize = 512);
    void save(const std::vector<ClusterData>& clusters, int version) override;
    bool load(std::vector<ClusterData>& clusters, int &version) override;
    platform::TimeMs lastTimestamp() const { return platform::TimeMs{_lastTimestamp}; }
    void clear() override;

    bool writeBlob(const std::vector<unsigned char>& data) override;
    bool readBlob(std::vector<unsigned char>& data) override;

private:
    size_t _startAddress;
    size_t _eepromSize;
    uint32_t _lastTimestamp{0};
    IPlatform& _platform;
};
