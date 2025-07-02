#include "FileCalibrationStorage.h"
#include <filesystem>
#include <fstream>
#include <ctime>

namespace fs = std::filesystem;

FileCalibrationStorage::FileCalibrationStorage(const std::string& path)
    : _path(path) {}

void FileCalibrationStorage::save(const std::vector<ClusterData>& clusters, int version) {
    if (fs::exists(_path)) {
        fs::rename(_path, _path + ".bak");
    }
    std::ofstream ofs(_path);
    _lastTimestamp = static_cast<uint32_t>(std::time(nullptr));
    ofs << version << " " << _lastTimestamp << "\n";
    for (const auto& c : clusters) {
        ofs << c.mean << " " << c.min << " " << c.max << " " << c.count << "\n";
    }
}

bool FileCalibrationStorage::load(std::vector<ClusterData>& clusters, int &version) {
    std::ifstream ifs(_path);
    if (!ifs)
        return false;
    clusters.clear();
    std::time_t ts;
    if (!(ifs >> version >> ts))
        return false;
    _lastTimestamp = static_cast<uint32_t>(ts);
    ClusterData c{};
    while (ifs >> c.mean >> c.min >> c.max >> c.count) {
        clusters.push_back(c);
    }
    return true;
}
