#include "SettingsManager.h"
#include <Calibration/CalibrationConfig.h>

SettingsManager::SettingsManager(ISettingsStorage& storage, IDiagnostics& diag)
    : _storage(storage), _data(), _diag(diag) {}

StorageResult SettingsManager::load() {
    StorageResult res = _storage.load(_data);
    ensureValid();
    if (res.ok()) {
        _diag.info("Settings loaded");
    } else {
        _diag.warn("Failed to load settings; using defaults");
    }
    return res;
}

void SettingsManager::apply(WindVane& vane) const {
    CalibrationConfig cfg{};
    cfg.spin = _data.spin;
    vane.setCalibrationConfig(cfg);
}

StorageResult SettingsManager::save() const {
    StorageResult res = _storage.save(_data);
    if (res.ok())
        _diag.info("Settings saved");
    else
        _diag.warn("Failed to save settings");
    return res;
}

float SettingsManager::spinThreshold() const { return _data.spin.threshold; }
int SettingsManager::spinBufferSize() const { return _data.spin.bufferSize; }
int SettingsManager::spinExpectedPositions() const { return _data.spin.expectedPositions; }
int SettingsManager::spinSampleDelayMs() const { return _data.spin.sampleDelayMs; }
int SettingsManager::spinStallTimeoutSec() const { return _data.spin.stallTimeoutSec; }

void SettingsManager::setSpinThreshold(float v) {
    _data.spin.threshold = v;
    ensureValid();
}
void SettingsManager::setSpinBufferSize(int v) {
    _data.spin.bufferSize = v;
    ensureValid();
}
void SettingsManager::setSpinExpectedPositions(int v) {
    _data.spin.expectedPositions = v;
    ensureValid();
}
void SettingsManager::setSpinSampleDelayMs(int v) {
    _data.spin.sampleDelayMs = v;
    ensureValid();
}
void SettingsManager::setSpinStallTimeoutSec(int v) {
    _data.spin.stallTimeoutSec = v;
    ensureValid();
}

void SettingsManager::ensureValid() {
    if (_data.spin.bufferSize < 1) _data.spin.bufferSize = 1;
    if (_data.spin.expectedPositions < 1) _data.spin.expectedPositions = 1;
    if (_data.spin.sampleDelayMs < 1) _data.spin.sampleDelayMs = 1;
    if (_data.spin.stallTimeoutSec < 1) _data.spin.stallTimeoutSec = 1;
    if (_data.spin.threshold < 0.f) _data.spin.threshold = 0.f;
}
