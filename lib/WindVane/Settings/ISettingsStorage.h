#pragma once
#include "SettingsData.h"

class ISettingsStorage {
public:
    virtual ~ISettingsStorage() = default;
    virtual void save(const SettingsData& data) = 0;
    virtual bool load(SettingsData& data) = 0;
};
