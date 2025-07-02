#pragma once

#include "ICalibrationStorage.h"
#include <string>

class FileCalibrationStorage : public ICalibrationStorage {
public:
    explicit FileCalibrationStorage(const std::string& path);
    void save(const std::vector<ClusterData>& clusters, int version) override;

private:
    std::string _path;
};
