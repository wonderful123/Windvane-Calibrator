#include "wind_vane/core/wind_vane.h"

#include <algorithm>
#include <cmath>

#include "wind_vane/core/calibration/calibration_manager.h"
#include "wind_vane/core/calibration/calibration_result.h"

namespace {
// Convert raw sensor value to degrees (0-360)
constexpr float kDegreesPerRevolution = 360.0f;
constexpr float kNormalizationFactor = 1.0f;

float NormalizeAngle(float angle) {
  while (angle < 0.0f) {
    angle += kDegreesPerRevolution;
  }
  while (angle >= kDegreesPerRevolution) {
    angle -= kDegreesPerRevolution;
  }
  return angle;
}
}  // namespace

WindVane::WindVane(const Config& config)
    : adc_(config.adc),
      storage_(config.storage),
      user_io_(config.user_io),
      diagnostics_(config.diagnostics) {
  
  // Initialize calibration manager with dependencies
  CalibrationManager::Config cal_config{
    .adc = adc_,
    .storage = storage_,
    .user_io = user_io_,
    .diagnostics = diagnostics_
  };
  
  calibration_manager_ = std::make_unique<CalibrationManager>(cal_config);
  
  diagnostics_.LogInfo("WindVane initialized successfully");
}

float WindVane::GetDirection() const {
  float raw_reading = GetRawReading();
  
  if (!IsCalibrated()) {
    diagnostics_.LogWarning("Wind vane not calibrated, returning raw reading");
    return NormalizeAngle(raw_reading * kDegreesPerRevolution);
  }
  
  return calibration_manager_->ApplyCalibration(raw_reading);
}

float WindVane::GetRawReading() const {
  if (!adc_.IsAvailable()) {
    diagnostics_.LogError("ADC not available for reading");
    return 0.0f;
  }
  
  float reading = adc_.ReadNormalized();
  
  // Validate reading is in expected range
  if (reading < 0.0f || reading > kNormalizationFactor) {
    diagnostics_.LogError("ADC reading out of range: " + std::to_string(reading));
    return std::clamp(reading, 0.0f, kNormalizationFactor);
  }
  
  return reading;
}

CalibrationResult WindVane::StartCalibration() {
  if (!calibration_manager_) {
    CalibrationResult result;
    result.success = false;
    result.error_message = "Calibration manager not initialized";
    return result;
  }
  
  diagnostics_.LogInfo("Starting wind vane calibration");
  CalibrationResult result = calibration_manager_->RunCalibration();
  
  if (result.success) {
    diagnostics_.LogInfo("Calibration completed successfully");
  } else {
    diagnostics_.LogError("Calibration failed: " + result.error_message);
  }
  
  return result;
}

bool WindVane::IsCalibrated() const {
  return calibration_manager_ && 
         calibration_manager_->HasValidCalibration();
}

bool WindVane::ClearCalibration() {
  if (!storage_) {
    diagnostics_.LogWarning("No storage available for clearing calibration");
    return false;
  }
  
  bool success = storage_->Clear();
  if (success) {
    diagnostics_.LogInfo("Calibration data cleared");
    // Reset calibration manager state
    if (calibration_manager_) {
      calibration_manager_->InvalidateCalibration();
    }
  } else {
    diagnostics_.LogError("Failed to clear calibration data");
  }
  
  return success;
}

uint64_t WindVane::GetLastCalibrationTimestamp() const {
  if (!storage_) {
    return 0;
  }
  
  return storage_->GetLastTimestamp();
}