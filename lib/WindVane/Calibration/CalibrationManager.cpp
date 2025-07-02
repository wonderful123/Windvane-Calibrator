#include "Calibration/CalibrationManager.h"
#include <iostream>

CalibrationManager::CalibrationManager(std::unique_ptr<ICalibrationStrategy> strategy,
                                       IIOHandler *io, IDiagnostics *diag)
    : calibrationStrategy(std::move(strategy)),
      status(CalibrationStatus::NotStarted),
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

bool CalibrationManager::runCalibration() {
  if (!startCalibration())
    return false;
  beginCalibration();
  // If calibration is aborted internally, status will be Completed already
  if (status == CalibrationStatus::InProgress)
    endCalibration();
  return true;
}

float CalibrationManager::getCalibratedData(float rawWindDirection) const {
  if (calibrationStrategy)
    return calibrationStrategy->mapReading(rawWindDirection);
  return rawWindDirection * 360.0f;
}

void CalibrationManager::editCalibrationData(/*data*/) {
  // Logic to edit calibration data
  // This could call methods on the calibrationStrategy to update its internal data
}

CalibrationManager::CalibrationStatus CalibrationManager::getStatus() const {
  return status;
}
