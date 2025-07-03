// lib/WindVane/WindVane.h

#pragma once

#include <memory>

#include "Calibration/CalibrationManager.h"
#include "Calibration/CalibrationMethod.h"
#include "Calibration/SpinningConfig.h"
#include "Calibration/Strategies/ISpinningConfigurable.h"
#include "Diagnostics/IDiagnostics.h"
#include "IADC.h"
#include "IO/IIO.h"
#include "Storage/ICalibrationStorage.h"

/**
 * @enum WindVaneType
 * @brief Enumeration for different types of wind vanes.
 */
enum class WindVaneType { REED_SWITCH };

/**
 * @struct WindVaneConfig
 * @brief All dependencies for custom advanced use.
 */
struct WindVaneConfig {
  IADC *adc{};
  WindVaneType type{WindVaneType::REED_SWITCH};
  CalibrationMethod method{CalibrationMethod::SPINNING};
  ICalibrationStorage *storage{};
  IIOHandler *io{};
  IDiagnostics *diag{};
  SpinningConfig config{};
};

class WindVane {
 public:
  // All dependencies must be provided via the configuration structure
  explicit WindVane(const WindVaneConfig &cfg);

  float direction();
  void calibrate();       // New: simple method alias for runCalibration()
  void runCalibration();  // Advanced
  uint32_t lastCalibrationTimestamp() const;
  CalibrationManager::CalibrationStatus calibrationStatus() const;
  void clearCalibration();
  void setCalibrationConfig(const SpinningConfig &cfg);
  SpinningConfig getCalibrationConfig() const;
  ICalibrationStorage *storage() const { return _storage; }

 private:
  float getRawDirection();
  IADC *_adc;
  WindVaneType _type;
  std::unique_ptr<CalibrationManager> _calibrationManager;
  ICalibrationStorage *_storage;
};
