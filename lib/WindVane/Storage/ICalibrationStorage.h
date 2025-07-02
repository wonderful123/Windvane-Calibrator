#pragma once
#include <vector>
#include "../Calibration/ClusterData.h"

class ICalibrationStorage {
public:
    virtual ~ICalibrationStorage() = default;
    virtual void save(const std::vector<ClusterData>& clusters, int version) = 0;
    virtual bool load(std::vector<ClusterData>& clusters, int &version) = 0;
};
