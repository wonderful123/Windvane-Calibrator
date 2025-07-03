// lib/WindVane/WindVane.cpp

#include "WindVane.h"

#include "Calibration/Strategies/ISpinningConfigurable.h"
#include "Calibration/StrategyFactory.h"
#include "Diagnostics/IDiagnostics.h"
#include "IO/IIOHandler.h"
#include "Storage/ICalibrationStorage.h"

#ifdef ARDUINO
#include <Diagnostics/SerialDiagnostics.h>
#include <Hardware/ESP32/ADC.h>
#include <IO/SerialIOHandler.h>
#include <Storage/EEPROMCalibrationStorage.h>
#else
#include <Diagnostics/ConsoleDiagnostics.h>
#include <IO/ConsoleIOHandler.h>
#include <Storage/FileCalibrationStorage.h>
#endif

// Existing full-config constructor (unchanged)
WindVane::WindVane(const WindVaneConfig& cfg)
    : _adc(cfg.adc), _type(cfg.type), _storage(cfg.storage) {
  StrategyContext ctx{cfg.method, cfg.adc,  cfg.storage,
                      cfg.io,     cfg.diag, cfg.config};
  auto strategy = createCalibrationStrategy(ctx);
  _calibrationManager = std::make_unique<CalibrationManager>(
      std::move(strategy), cfg.io, cfg.diag);
}

// --- NEW MINIMALIST CONSTRUCTOR ---
WindVane::WindVane(int adcPin, unsigned long serialBaud) {
#ifdef ARDUINO
  static ESP32ADC _default_adc(adcPin);
  static EEPROMCalibrationStorage _default_storage(0, 512);
  static SerialDiagnostics _default_diag;
  static SerialIOHandler _default_io;
  static SpinningConfig _default_config;  // Zero/default values
  static WindVaneConfig _default_cfg{&_default_adc,
                                     WindVaneType::REED_SWITCH,
                                     CalibrationMethod::SPINNING,
                                     &_default_storage,
                                     &_default_io,
                                     &_default_diag,
                                     _default_config};
  Serial.begin(serialBaud);
  *this = WindVane(_default_cfg);
#else
  // Host version (if you want Console support)
  static ConsoleIOHandler _default_io;
  static FileCalibrationStorage _default_storage("calib.dat");
  static ConsoleDiagnostics _default_diag;
  static SpinningConfig _default_config;
  static WindVaneConfig _default_cfg{
      nullptr,  // User must set an IADC for desktop/host
      WindVaneType::REED_SWITCH,
      CalibrationMethod::SPINNING,
      &_default_storage,
      &_default_io,
      &_default_diag,
      _default_config};
  *this = WindVane(_default_cfg);
#endif
}

// --- New: user-friendly alias for calibration ---
void WindVane::calibrate() { runCalibration(); }
#include "Calibration/Strategies/ISpinningConfigurable.h"

float WindVane::direction() {
  float raw = getRawDirection();
  return _calibrationManager ? _calibrationManager->getCalibratedData(raw)
                             : raw * 360.0f;
}

float WindVane::getRawDirection() { return _adc ? _adc->read() : 0.0f; }

void WindVane::runCalibration() {
  if (_calibrationManager) _calibrationManager->runCalibration();
}

CalibrationManager::CalibrationStatus WindVane::calibrationStatus() const {
  return _calibrationManager
             ? _calibrationManager->getStatus()
             : CalibrationManager::CalibrationStatus::NotStarted;
}

uint32_t WindVane::lastCalibrationTimestamp() const {
  return _storage ? _storage->lastTimestamp() : 0;
}

void WindVane::clearCalibration() {
  if (_storage) _storage->clear();
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