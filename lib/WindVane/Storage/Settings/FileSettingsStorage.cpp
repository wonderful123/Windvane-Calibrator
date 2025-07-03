#include "FileSettingsStorage.h"
#include <fstream>

FileSettingsStorage::FileSettingsStorage(const std::string& path)
    : _path(path) {}

StorageResult FileSettingsStorage::save(const SettingsData& data) {
    std::ofstream ofs(_path);
    if (!ofs)
        return {StorageStatus::IoError, "open"};
    ofs << data.spin.threshold << " " << data.spin.bufferSize << " "
        << data.spin.expectedPositions << " " << data.spin.sampleDelayMs << " "
        << data.spin.stallTimeoutSec << "\n";
    if (!ofs)
        return {StorageStatus::IoError, "write"};
    return {};
}

StorageResult FileSettingsStorage::load(SettingsData& data) {
    std::ifstream ifs(_path);
    if (!ifs)
        return {StorageStatus::NotFound, "open"};
    ifs >> data.spin.threshold >> data.spin.bufferSize >> data.spin.expectedPositions
        >> data.spin.sampleDelayMs >> data.spin.stallTimeoutSec;
    if (!ifs)
        return {StorageStatus::IoError, "read"};
    return {};
}
