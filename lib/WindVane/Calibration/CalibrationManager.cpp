#include "Calibration/CalibrationManager.h"

CalibrationManager::CalibrationManager(ICalibrationStrategy* strategy)
  : calibrationStrategy(strategy), status(CalibrationStatus::NotStarted) {}

void CalibrationManager::startCalibration() {
  // Logic to start the calibration
  status = CalibrationStatus::AwaitingStart;
  // Send confirmation message and instructions (via Serial, etc.)
}

void CalibrationManager::beginCalibration() {
  // Logic to actually begin calibration
  status = CalibrationStatus::InProgress;
  // Start the calibration using the strategy
}

void CalibrationManager::endCalibration() {
  // Logic to end calibration
  status = CalibrationStatus::Completed;
  // Finalize calibration and store data
}

void CalibrationManager::getCalibratedData(float rawWindDirection) {
  // Retrieve the calibrated data
}

void CalibrationManager::editCalibrationData(/*data*/) {
  // Logic to edit calibration data
  // This could call methods on the calibrationStrategy to update its internal data
}

CalibrationManager::CalibrationStatus CalibrationManager::getStatus() const {
  return status;
}