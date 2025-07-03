// lib/WindVane/WindVane.h

#pragma once

#include <memory>

#include "Calibration/CalibrationManager.h"
#include "Calibration/CalibrationResult.h"
#include "Calibration/CalibrationMethod.h"
#include "Calibration/CalibrationConfig.h"
#include "Diagnostics/IDiagnostics.h"
#include "IADC.h"
#include "UI/IIO.h"
#include "Storage/ICalibrationStorage.h"
#include "Storage/StorageResult.h"
#include <Platform/TimeUtils.h>

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
  IADC& adc;
  WindVaneType type{WindVaneType::REED_SWITCH};
  CalibrationMethod method{CalibrationMethod::SPINNING};
  ICalibrationStorage* storage{nullptr};
  IUserIO& io;
  IDiagnostics& diag;
  CalibrationConfig config{};
};

class WindVane {
 public:
  // All dependencies must be provided via the configuration structure
  explicit WindVane(const WindVaneConfig &cfg);

  float getDirection() const;
  CalibrationResult calibrate();       // New: simple method alias for runCalibration()
  CalibrationResult runCalibration();  // Advanced
  platform::TimeMs getLastCalibrationTimestamp() const;
  CalibrationManager::CalibrationStatus getCalibrationStatus() const;
  StorageResult clearCalibration() const;
  void setCalibrationConfig(const CalibrationConfig &cfg);
  CalibrationConfig getCalibrationConfig() const;
  ICalibrationStorage *getStorage() const { return _storage; }

 private:
  float getRawDirection() const;
  IADC& _adc;
  WindVaneType _type;
  std::unique_ptr<CalibrationManager> _calibrationManager;
  ICalibrationStorage* _storage;
};
