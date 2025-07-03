#pragma once
#include "CalibrationStorageBase.h"
#include "IBlobStorage.h"
#include "StorageResult.h"
#include <Platform/TimeUtils.h>
#include <Platform/IPlatform.h>
#include <cstdint>
#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMCalibrationStorage final : public CalibrationStorageBase, public IBlobStorage {
public:
    EEPROMCalibrationStorage(IPlatform& platform,
                             size_t startAddress = 0,
                             size_t eepromSize = 512);
    StorageResult save(const std::vector<ClusterData>& clusters, int version) override;
    StorageResult load(std::vector<ClusterData>& clusters, int &version) override;
    platform::TimeMs lastTimestamp() const override { return platform::TimeMs{_lastTimestamp}; }
    void clear() override;

    StorageResult writeBlob(const std::vector<unsigned char>& data) override;
    StorageResult readBlob(std::vector<unsigned char>& data) override;

private:
    size_t _startAddress;
    size_t _eepromSize;
    uint32_t _lastTimestamp{0};
    IPlatform& _platform;
};
