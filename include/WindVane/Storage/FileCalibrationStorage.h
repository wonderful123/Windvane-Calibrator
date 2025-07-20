#pragma once

#include "CalibrationStorageBase.h"
#include "IBlobStorage.h"
#include "StorageResult.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

class FileCalibrationStorage final : public CalibrationStorageBase, public IBlobStorage {
public:
    explicit FileCalibrationStorage(const std::string& path);
    StorageResult save(const std::vector<ClusterData>& clusters, int version) override;
    StorageResult load(std::vector<ClusterData>& clusters, int &version) override;
    StorageResult clear() override;

    StorageResult writeBlob(const std::vector<unsigned char>& data) override;
    StorageResult readBlob(std::vector<unsigned char>& data) override;

private:
    std::string _path;

    void backupExisting() const;
};
