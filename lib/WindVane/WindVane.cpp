#include "WindVane.h"
#include "Calibration/StrategyFactory.h"
#include "Storage/ICalibrationStorage.h"
#include "IO/IIOHandler.h"
#include "Diagnostics/IDiagnostics.h"
#include "Calibration/Strategies/ISpinningConfigurable.h"

WindVane::WindVane(const WindVaneConfig &cfg)
    : _adc(cfg.adc), _type(cfg.type), _storage(cfg.storage) {
    StrategyContext ctx{cfg.method, cfg.adc, cfg.storage, cfg.io, cfg.diag, cfg.config};
    auto strategy = createCalibrationStrategy(ctx);
    _calibrationManager =
        std::make_unique<CalibrationManager>(std::move(strategy), cfg.io, cfg.diag);
}

float WindVane::direction() {
    float raw = getRawDirection();
    return _calibrationManager ? _calibrationManager->getCalibratedData(raw)
                               : raw * 360.0f;
}

float WindVane::getRawDirection() {
    return _adc ? _adc->read() : 0.0f;
}

void WindVane::runCalibration() {
    if (_calibrationManager)
        _calibrationManager->runCalibration();
}

CalibrationManager::CalibrationStatus WindVane::calibrationStatus() const {
    return _calibrationManager ? _calibrationManager->getStatus()
                               : CalibrationManager::CalibrationStatus::NotStarted;
}

uint32_t WindVane::lastCalibrationTimestamp() const {
    return _storage ? _storage->lastTimestamp() : 0;
}

void WindVane::clearCalibration() {
    if (_storage)
        _storage->clear();
}

void WindVane::setCalibrationConfig(const SpinningConfig& cfg) {
  if (_calibrationManager) {
    ICalibrationStrategy* strat = _calibrationManager->strategy();
    if (strat->strategyType() == CalibrationStrategyType::Spinning) {
      reinterpret_cast<ISpinningConfigurable*>(strat)->setConfig(cfg);
    }
  }
}

SpinningConfig WindVane::getCalibrationConfig() const {
  if (_calibrationManager) {
    ICalibrationStrategy* strat = _calibrationManager->strategy();
    if (strat->strategyType() == CalibrationStrategyType::Spinning) {
      return reinterpret_cast<ISpinningConfigurable*>(strat)->config();
    }
  }
  return {};
}
