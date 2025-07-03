#include "Calibration/CalibrationManager.h"

CalibrationManager::CalibrationManager(std::unique_ptr<ICalibrationStrategy> strategy)
    : calibrationStrategy(std::move(strategy)),
      status(CalibrationStatus::NotStarted) {}

CalibrationResult CalibrationManager::beginCalibration() {
  CalibrationResult result{};
  if (status != CalibrationStatus::NotStarted &&
      status != CalibrationStatus::Completed) {
    result.error = "Calibration already running";
    return result;
  }
  status = CalibrationStatus::InProgress;
  if (calibrationStrategy) {
    calibrationStrategy->calibrate();
    result.success = true;
  } else {
    result.error = "No strategy";
  }
  status = CalibrationStatus::Completed;
  return result;
}

CalibrationResult CalibrationManager::runCalibration() {
  return beginCalibration();
}

float CalibrationManager::getCalibratedData(float rawWindDirection) const {
  if (calibrationStrategy)
    return calibrationStrategy->mapReading(rawWindDirection);
  return rawWindDirection * 360.0f;
}

void CalibrationManager::editCalibrationData(/*data*/) {}

CalibrationManager::CalibrationStatus CalibrationManager::getStatus() const {
  return status;
}

