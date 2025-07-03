#include "FileCalibrationStorage.h"
#include <filesystem>
#include <fstream>
#include <ctime>

namespace fs = std::filesystem;

FileCalibrationStorage::FileCalibrationStorage(const std::string& path)
    : _path(path) {}

void FileCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
    backupExisting();
    std::ofstream ofs(_path);
    if (!ofs)
        return;
    _lastTimestamp = static_cast<uint32_t>(std::time(nullptr));
    writeHeader(ofs, version);
    writeClusters(ofs, clusters);
}

bool FileCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
    std::ifstream ifs(_path);
    if (!ifs)
        return false;
    clusters.clear();
    if (!readHeader(ifs, version))
        return false;
    readClusters(ifs, clusters);
    if (!ifs)
        return false;
    return true;
}

void FileCalibrationStorage::clear() {
    std::error_code ec;
    std::filesystem::remove(_path, ec);
    _lastTimestamp = 0;
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

void FileCalibrationStorage::writeHeader(std::ofstream& ofs, int version) {
    ofs << version << " " << _lastTimestamp << "\n";
}

void FileCalibrationStorage::writeClusters(std::ofstream& ofs, const std::vector<ClusterData>& clusters) {
    for (const auto& c : clusters) {
        ofs << c.mean << " " << c.min << " " << c.max << " " << c.count << "\n";
    }
}

bool FileCalibrationStorage::readHeader(std::ifstream& ifs, int& version) {
    std::time_t ts;
    if (!(ifs >> version >> ts))
        return false;
    _lastTimestamp = static_cast<uint32_t>(ts);
    return true;
}

void FileCalibrationStorage::readClusters(std::ifstream& ifs, std::vector<ClusterData>& clusters) {
    ClusterData c{};
    while (ifs >> c.mean >> c.min >> c.max >> c.count) {
        clusters.push_back(c);
    }
}
