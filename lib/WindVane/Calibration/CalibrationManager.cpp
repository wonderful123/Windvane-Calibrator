#include "Calibration/CalibrationManager.h"
#include <iostream>

CalibrationManager::CalibrationManager(ICalibrationStrategy *strategy,
                                       IIOHandler *io, IDiagnostics *diag)
    : calibrationStrategy(strategy), status(CalibrationStatus::NotStarted),
      _io(io), _diag(diag) {}

bool CalibrationManager::startCalibration() {
  if (status == CalibrationStatus::InProgress)
    return false;
  status = CalibrationStatus::AwaitingStart;
  if (_diag)
    _diag->info("Ready to calibrate. Press any key to start.");
  return true;
}


bool CalibrationManager::beginCalibration() {
  if (status != CalibrationStatus::AwaitingStart)
    return false;
  if (_io) {
    while (!_io->hasInput())
      _io->waitMs(10);
    _io->flushInput();
  }
  status = CalibrationStatus::InProgress;
  if (calibrationStrategy)
    calibrationStrategy->calibrate();
  status = CalibrationStatus::Completed;
  if (_diag)
    _diag->info("Calibration finished.");
  return true;
}

bool CalibrationManager::endCalibration() {
  if (status != CalibrationStatus::InProgress)
    return false;
  status = CalibrationStatus::Completed;
  if (_diag)
    _diag->info("Calibration ended.");
  return true;
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
