#pragma once
#include "ICalibrationStorage.h"
#include <cstdint>
#include <vector>

struct CalibrationStorageHeader {
    uint16_t version;
    uint32_t timestamp;
    uint16_t count;
    uint32_t crc;
};

class CalibrationStorageBase : public ICalibrationStorage {
public:
    CalibrationStorageBase() = default;
    int getSchemaVersion() const override { return _schemaVersion; }
    platform::TimeMs lastTimestamp() const override { return platform::TimeMs{_lastTimestamp}; }
protected:
    int _schemaVersion{0};
    uint32_t _lastTimestamp{0};

    static uint32_t crc32(const unsigned char* data, size_t len);
    static uint32_t crc32(const std::vector<ClusterData>& clusters);
};
