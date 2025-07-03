#pragma once

#include "ICalibrationStorage.h"
#include <string>
#include <cstdint>

class FileCalibrationStorage : public ICalibrationStorage {
public:
    explicit FileCalibrationStorage(const std::string& path);
    void save(const std::vector<ClusterData>& clusters, int version) override;
    bool load(std::vector<ClusterData>& clusters, int &version) override;
    uint32_t lastTimestamp() const override { return _lastTimestamp; }
    void clear() override;

private:
    std::string _path;
    uint32_t _lastTimestamp{0};
};
