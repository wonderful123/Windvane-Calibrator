#include "FileSettingsStorage.h"
#include <fstream>
#include <cstdint>

FileSettingsStorage::FileSettingsStorage(const std::string& path)
    : _path(path) {}

StorageResult FileSettingsStorage::save(const SettingsData& data) {
    std::ofstream ofs(_path, std::ios::binary);
    if (!ofs)
        return {StorageStatus::IoError, "open"};
    SettingsStorageHeader hdr{};
    hdr.version = 1;
    hdr.crc = SettingsStorageBase::crc32(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
    ofs.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
    ofs.write(reinterpret_cast<const char*>(&data), sizeof(data));
    if (!ofs)
        return {StorageStatus::IoError, "write"};
    _schemaVersion = hdr.version;
    return {};
}

StorageResult FileSettingsStorage::load(SettingsData& data) {
    std::ifstream ifs(_path, std::ios::binary);
    if (!ifs)
        return {StorageStatus::NotFound, "open"};
    SettingsStorageHeader hdr{};
    ifs.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
    if (!ifs)
        return {StorageStatus::IoError, "header"};
    ifs.read(reinterpret_cast<char*>(&data), sizeof(data));
    if (!ifs)
        return {StorageStatus::IoError, "data"};
    uint32_t crc = SettingsStorageBase::crc32(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
    if (crc != hdr.crc)
        return {StorageStatus::CorruptData, "crc"};
    _schemaVersion = hdr.version;
    return {};
}
