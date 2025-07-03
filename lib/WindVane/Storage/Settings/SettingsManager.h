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

    const SettingsData& data() const { return _data; }

    // Spinning configuration getters/setters
    float spinThreshold() const;
    int spinBufferSize() const;
    int spinExpectedPositions() const;
    int spinSampleDelayMs() const;
    int spinStallTimeoutSec() const;

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
