#include "Calibration/CalibrationManager.h"
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#endif

CalibrationManager::CalibrationManager(ICalibrationStrategy *strategy)
    : calibrationStrategy(strategy), status(CalibrationStatus::NotStarted) {}

void CalibrationManager::startCalibration() {
  status = CalibrationStatus::AwaitingStart;
#ifdef ARDUINO
  Serial.println(F("Ready to calibrate. Press a key to start spinning."));
#else
  std::cout << "Ready to calibrate. Begin spinning." << std::endl;
#endif
}


void CalibrationManager::beginCalibration() {
  status = CalibrationStatus::InProgress;
  if (calibrationStrategy)
    calibrationStrategy->calibrate();
  status = CalibrationStatus::Completed;
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