#include "SettingsManager.h"
#include <Calibration/CalibrationConfig.h>

SettingsManager::SettingsManager(ISettingsStorage& storage, SettingsData& data,
                                 IDiagnostics& diag)
    : _storage(storage), _data(data), _diag(diag) {}

bool SettingsManager::load() {
    if (_storage.load(_data)) {
        _diag.info("Settings loaded");
        return true;
    }
    _diag.warn("Failed to load settings; using defaults");
    return false;
}

void SettingsManager::apply(WindVane& vane) const {
    CalibrationConfig cfg{};
    cfg.spin = _data.spin;
    vane.setCalibrationConfig(cfg);
}

void SettingsManager::save() const {
    _storage.save(_data);
    _diag.info("Settings saved");
}
