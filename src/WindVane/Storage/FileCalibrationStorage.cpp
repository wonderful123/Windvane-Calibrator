#include "FileCalibrationStorage.h"
#include <filesystem>
#include <fstream>
#include <ctime>

namespace fs = std::filesystem;

FileCalibrationStorage::FileCalibrationStorage(const std::string& path)
    : _path(path) {}

StorageResult FileCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
    backupExisting();
    std::ofstream ofs(_path, std::ios::binary);
    if (!ofs)
        return {StorageStatus::IoError, "open"};
    
    // Check for overflow before casting to uint16_t
    if (clusters.size() > UINT16_MAX) {
        return {StorageStatus::InvalidFormat, "too many clusters"};
    }
    
    _lastTimestamp = static_cast<uint32_t>(std::time(nullptr));
    _schemaVersion = version;
    CalibrationStorageHeader hdr{};
    hdr.version = static_cast<uint16_t>(version);
    hdr.timestamp = _lastTimestamp;
    hdr.count = static_cast<uint16_t>(clusters.size());
    hdr.crc = crc32(clusters);
    ofs.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
    ofs.write(reinterpret_cast<const char*>(clusters.data()),
              clusters.size() * sizeof(ClusterData));
    if (!ofs)
        return {StorageStatus::IoError, "write"};
    return {};
}

StorageResult FileCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
    std::ifstream ifs(_path, std::ios::binary);
    if (!ifs)
        return {StorageStatus::NotFound, "open"};
    clusters.clear();
    CalibrationStorageHeader hdr{};
    ifs.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
    if (!ifs)
        return {StorageStatus::IoError, "header"};
    version = hdr.version;
    _schemaVersion = hdr.version;
    _lastTimestamp = hdr.timestamp;
    
    // Validate count to prevent buffer overflow
    if (hdr.count == 0 || hdr.count > 1024) {  // Reasonable upper limit
        return {StorageStatus::InvalidFormat, "invalid cluster count"};
    }
    
    clusters.resize(hdr.count);
    ifs.read(reinterpret_cast<char*>(clusters.data()), hdr.count * sizeof(ClusterData));
    if (!ifs)
        return {StorageStatus::IoError, "data"};
    uint32_t crc = crc32(clusters);
    if (crc != hdr.crc)
        return {StorageStatus::CorruptData, "crc"};
    return {};
}

StorageResult FileCalibrationStorage::clear() {
    std::error_code ec;
    std::filesystem::remove(_path, ec);
    _lastTimestamp = 0;
    _schemaVersion = 0;
    if (ec)
        return {StorageStatus::IoError, ec.message()};
    return {};
}

StorageResult FileCalibrationStorage::writeBlob(const std::vector<unsigned char>& data) {
    std::ofstream ofs(_path, std::ios::binary);
    if (!ofs)
        return {StorageStatus::IoError, "open"};
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
    if (!ofs)
        return {StorageStatus::IoError, "write"};
    return {};
}

StorageResult FileCalibrationStorage::readBlob(std::vector<unsigned char>& data) {
    std::ifstream ifs(_path, std::ios::binary);
    if (!ifs)
        return {StorageStatus::NotFound, "open"};
    data.assign(std::istreambuf_iterator<char>(ifs),
                std::istreambuf_iterator<char>());
    if (!ifs)
        return {StorageStatus::IoError, "read"};
    return {};
}

void FileCalibrationStorage::backupExisting() const {
    if (fs::exists(_path)) {
        fs::rename(_path, _path + ".bak");
    }
}

