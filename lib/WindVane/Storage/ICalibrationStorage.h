#pragma once
#include <vector>
#include <cstdint>
#include <Platform/TimeUtils.h>
#include "../Calibration/ClusterData.h"
#include "StorageResult.h"

class ICalibrationStorage {
public:
    virtual ~ICalibrationStorage() = default;
    virtual StorageResult save(const std::vector<ClusterData>& clusters, int version) = 0;
    virtual StorageResult load(std::vector<ClusterData>& clusters, int &version) = 0;
    virtual int getSchemaVersion() const = 0;
    virtual platform::TimeMs lastTimestamp() const { return platform::TimeMs{0}; }
    virtual void clear() = 0;
};
