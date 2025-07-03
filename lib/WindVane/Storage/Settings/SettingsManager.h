#pragma once
#include "SettingsData.h"
#include "ISettingsStorage.h"
#include <Diagnostics/IDiagnostics.h>
#include <WindVane.h>

/** Manages loading, applying and saving device settings. */
class SettingsManager {
public:
    SettingsManager(ISettingsStorage& storage, SettingsData& data, IDiagnostics& diag);

    bool load();
    void apply(WindVane& vane) const;
    void save() const;

    SettingsData& data() { return _data; }
private:
    ISettingsStorage& _storage;
    SettingsData& _data;
    IDiagnostics& _diag;
};
