#pragma once
#include "SettingsData.h"
#include "../StorageResult.h"

class ISettingsStorage {
public:
    virtual ~ISettingsStorage() = default;
    virtual StorageResult save(const SettingsData& data) = 0;
    virtual StorageResult load(SettingsData& data) = 0;
};
