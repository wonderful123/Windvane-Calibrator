#pragma once
#include "ISettingsStorage.h"
#include <string>

class FileSettingsStorage : public ISettingsStorage {
public:
    explicit FileSettingsStorage(const std::string& path);
    void save(const SettingsData& data) override;
    bool load(SettingsData& data) override;
private:
    std::string _path;
};
