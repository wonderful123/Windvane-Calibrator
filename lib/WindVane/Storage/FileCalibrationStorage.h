#pragma once

#include "ICalibrationStorage.h"
#include "IBlobStorage.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

class FileCalibrationStorage : public ICalibrationStorage, public IBlobStorage {
public:
    explicit FileCalibrationStorage(const std::string& path);
    void save(const std::vector<ClusterData>& clusters, int version) override;
    bool load(std::vector<ClusterData>& clusters, int &version) override;
    platform::TimeMs lastTimestamp() const override { return platform::TimeMs{_lastTimestamp}; }
    void clear() override;

    bool writeBlob(const std::vector<unsigned char>& data) override;
    bool readBlob(std::vector<unsigned char>& data) override;

private:
    std::string _path;
    uint32_t _lastTimestamp{0};

    void backupExisting() const;
    void writeHeader(std::ofstream& ofs, int version) const;
    void writeClusters(std::ofstream& ofs, const std::vector<ClusterData>& clusters) const;
    bool readHeader(std::ifstream& ifs, int& version);
    void readClusters(std::ifstream& ifs, std::vector<ClusterData>& clusters) const;
};
