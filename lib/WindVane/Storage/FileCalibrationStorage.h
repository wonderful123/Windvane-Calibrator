#pragma once

#include "ICalibrationStorage.h"
#include <vector>
#include <fstream>
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

    void backupExisting() const;
    void writeHeader(std::ofstream& ofs, int version);
    void writeClusters(std::ofstream& ofs, const std::vector<ClusterData>& clusters);
    bool readHeader(std::ifstream& ifs, int& version);
    void readClusters(std::ifstream& ifs, std::vector<ClusterData>& clusters);
};
