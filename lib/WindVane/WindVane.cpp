// lib/WindVane/WindVane.cpp

#include "WindVane.h"

#include "Calibration/StrategyFactory.h"
#include "Calibration/CalibrationConfig.h"
#include "Diagnostics/IDiagnostics.h"
#include "UI/IIO.h"
#include "Storage/ICalibrationStorage.h"

// No default hardware instantiation here; all dependencies must be injected

// Existing full-config constructor (unchanged)
WindVane::WindVane(const WindVaneConfig& cfg)
    : _adc(cfg.adc), _type(cfg.type), _storage(cfg.storage) {
  StrategyContext ctx{cfg.method, cfg.adc, cfg.storage,
                      cfg.diag, cfg.config};
  auto strategy = createCalibrationStrategy(ctx);
  _calibrationManager = std::make_unique<CalibrationManager>(
      std::move(strategy));
}

// --- New: user-friendly alias for calibration ---
CalibrationResult WindVane::calibrate() { return runCalibration(); }

float WindVane::direction() const {
  float raw = getRawDirection();
  return _calibrationManager ? _calibrationManager->getCalibratedData(raw)
                             : raw * 360.0f;
}

float WindVane::getRawDirection() const { return _adc.read(); }

CalibrationResult WindVane::runCalibration() {
  if (_calibrationManager) return _calibrationManager->runCalibration();
  return {};
}

CalibrationManager::CalibrationStatus WindVane::calibrationStatus() const {
  return _calibrationManager
             ? _calibrationManager->getStatus()
             : CalibrationManager::CalibrationStatus::NotStarted;
}

platform::TimeMs WindVane::lastCalibrationTimestamp() const {
  return _storage ? platform::TimeMs{_storage->lastTimestamp()} : platform::TimeMs{0};
}

void WindVane::clearCalibration() const {
  if (_storage) _storage->clear();
}

void WindVane::setCalibrationConfig(const CalibrationConfig& cfg) {
  if (_calibrationManager) {
    _calibrationManager->strategy()->setConfig(cfg);
  }
}

CalibrationConfig WindVane::getCalibrationConfig() const {
  if (_calibrationManager) {
    return _calibrationManager->strategy()->config();
  }
  return CalibrationConfig{};
}
