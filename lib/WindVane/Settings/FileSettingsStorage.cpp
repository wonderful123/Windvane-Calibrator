#include "FileSettingsStorage.h"
#include <fstream>

FileSettingsStorage::FileSettingsStorage(const std::string& path)
    : _path(path) {}

void FileSettingsStorage::save(const SettingsData& data) {
    std::ofstream ofs(_path);
    ofs << data.spin.threshold << " " << data.spin.bufferSize << " "
        << data.spin.expectedPositions << " " << data.spin.sampleDelayMs << " "
        << data.spin.stallTimeoutSec << "\n";
}

bool FileSettingsStorage::load(SettingsData& data) {
    std::ifstream ifs(_path);
    if (!ifs)
        return false;
    ifs >> data.spin.threshold >> data.spin.bufferSize >> data.spin.expectedPositions
        >> data.spin.sampleDelayMs >> data.spin.stallTimeoutSec;
    return static_cast<bool>(ifs);
}
