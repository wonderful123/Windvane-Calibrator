#pragma once
#include "ISettingsStorage.h"
#include <string>
#include "../StorageResult.h"

class FileSettingsStorage : public ISettingsStorage {
public:
    explicit FileSettingsStorage(const std::string& path);
    StorageResult save(const SettingsData& data) override;
    StorageResult load(SettingsData& data) override;
private:
    std::string _path;
};
