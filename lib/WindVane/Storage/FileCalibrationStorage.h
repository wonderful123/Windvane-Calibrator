#pragma once

#include "CalibrationStorageBase.h"
#include "IBlobStorage.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

class FileCalibrationStorage final : public CalibrationStorageBase, public IBlobStorage {
public:
    explicit FileCalibrationStorage(const std::string& path);
    void save(const std::vector<ClusterData>& clusters, int version) override;
    bool load(std::vector<ClusterData>& clusters, int &version) override;
    void clear() override;

    bool writeBlob(const std::vector<unsigned char>& data) override;
    bool readBlob(std::vector<unsigned char>& data) override;

private:
    std::string _path;

    void backupExisting() const;
};
