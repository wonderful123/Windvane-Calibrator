#include "FileCalibrationStorage.h"
#include <filesystem>
#include <fstream>
#include <ctime>

namespace fs = std::filesystem;

FileCalibrationStorage::FileCalibrationStorage(const std::string& path)
    : _path(path) {}

void FileCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
    backupExisting();
    std::ofstream ofs(_path, std::ios::binary);
    if (!ofs)
        return;
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
}

bool FileCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
    std::ifstream ifs(_path, std::ios::binary);
    if (!ifs)
        return false;
    clusters.clear();
    CalibrationStorageHeader hdr{};
    ifs.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
    if (!ifs)
        return false;
    version = hdr.version;
    _schemaVersion = hdr.version;
    _lastTimestamp = hdr.timestamp;
    clusters.resize(hdr.count);
    ifs.read(reinterpret_cast<char*>(clusters.data()), hdr.count * sizeof(ClusterData));
    if (!ifs)
        return false;
    uint32_t crc = crc32(clusters);
    if (crc != hdr.crc)
        return false;
    return true;
}

void FileCalibrationStorage::clear() {
    std::error_code ec;
    std::filesystem::remove(_path, ec);
    _lastTimestamp = 0;
    _schemaVersion = 0;
}

bool FileCalibrationStorage::writeBlob(const std::vector<unsigned char>& data) {
    std::ofstream ofs(_path, std::ios::binary);
    if (!ofs)
        return false;
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
    return static_cast<bool>(ofs);
}

bool FileCalibrationStorage::readBlob(std::vector<unsigned char>& data) {
    std::ifstream ifs(_path, std::ios::binary);
    if (!ifs)
        return false;
    data.assign(std::istreambuf_iterator<char>(ifs),
                std::istreambuf_iterator<char>());
    return static_cast<bool>(ifs);
}

void FileCalibrationStorage::backupExisting() const {
    if (fs::exists(_path)) {
        fs::rename(_path, _path + ".bak");
    }
}

