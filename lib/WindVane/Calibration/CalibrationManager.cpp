#include "Calibration/CalibrationManager.h"
#include <iostream>

CalibrationManager::CalibrationManager(std::unique_ptr<ICalibrationStrategy> strategy,
                                       IIOHandler *io, IDiagnostics *diag)
    : calibrationStrategy(std::move(strategy)),
      status(CalibrationStatus::NotStarted),
      _io(io), _diag(diag) {}

bool CalibrationManager::beginCalibration() {
  if (status != CalibrationStatus::NotStarted &&
      status != CalibrationStatus::Completed)
    return false;
  status = CalibrationStatus::AwaitingStart;
  if (_diag)
    _diag->info("Ready to calibrate. Press any key to start.");
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

bool CalibrationManager::runCalibration() {
  return beginCalibration();
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
