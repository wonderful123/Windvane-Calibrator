#pragma once
#include "SettingsData.h"
#include "ISettingsStorage.h"
#include "../StorageResult.h"
#include <Diagnostics/IDiagnostics.h>
#include <WindVane.h>

/** Manages loading, applying and saving device settings. */
class SettingsManager {
public:
    SettingsManager(ISettingsStorage& storage, IDiagnostics& diag);

    StorageResult load();
    void apply(WindVane& vane) const;
    StorageResult save() const;

    const SettingsData& getData() const { return _data; }

    // Spinning configuration getters/setters
    float getSpinThreshold() const;
    int getSpinBufferSize() const;
    int getSpinExpectedPositions() const;
    int getSpinSampleDelayMs() const;
    int getSpinStallTimeoutSec() const;

    PersistedMenuState getMenuState() const { return _data.menuState; }
    void setMenuState(PersistedMenuState s);

    void setSpinThreshold(float v);
    void setSpinBufferSize(int v);
    void setSpinExpectedPositions(int v);
    void setSpinSampleDelayMs(int v);
    void setSpinStallTimeoutSec(int v);

private:
    void ensureValid();

    ISettingsStorage& _storage;
    SettingsData _data;
    IDiagnostics& _diag;
};
