#pragma once
#include <vector>
#include <cstdint>
#include <Platform/TimeUtils.h>
#include "../Calibration/ClusterData.h"

class ICalibrationStorage {
public:
    virtual ~ICalibrationStorage() = default;
    virtual void save(const std::vector<ClusterData>& clusters, int version) = 0;
    virtual bool load(std::vector<ClusterData>& clusters, int &version) = 0;
    virtual int getSchemaVersion() const = 0;
    virtual platform::TimeMs lastTimestamp() const { return platform::TimeMs{0}; }
    virtual void clear() = 0;
};
