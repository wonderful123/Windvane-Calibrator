#include "SettingsManager.h"

bool SettingsManager::load() {
    if (_storage && _data) {
        if (_storage->load(*_data)) {
            if (_diag) _diag->info("Settings loaded");
            return true;
        } else {
            if (_diag) _diag->warn("Failed to load settings; using defaults");
        }
    }
    return false;
}

void SettingsManager::apply(WindVane& vane) const {
    if (_data)
        vane.setCalibrationConfig(_data->spin);
}

void SettingsManager::save() const {
    if (_storage && _data) {
        _storage->save(*_data);
        if (_diag) _diag->info("Settings saved");
    }
}
