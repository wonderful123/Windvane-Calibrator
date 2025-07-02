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
    ofs << version << " " << std::time(nullptr) << "\n";
    for (const auto& c : clusters) {
        ofs << c.mean << " " << c.min << " " << c.max << " " << c.count << "\n";
    }
}
