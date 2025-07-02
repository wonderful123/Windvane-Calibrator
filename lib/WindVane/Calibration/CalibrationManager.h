#pragma once
#include "Calibration/Strategies/ICalibrationStrategy.h"
#include "../IO/IIOHandler.h"
#include "../Diagnostics/IDiagnostics.h"
#include <memory>

class CalibrationManager {
public:
  enum class CalibrationStatus {
    NotStarted,
    AwaitingStart,
    InProgress,
    Completed
  };

  CalibrationManager(std::unique_ptr<ICalibrationStrategy> strategy,
                     IIOHandler *io, IDiagnostics *diag);

  // Prepares the device for calibration and waits for user confirmation
  bool beginCalibration();

  // Runs the full calibration process in a single step
  bool runCalibration();

  // Converts a raw wind reading to calibrated degrees
  float getCalibratedData(float rawWindDirection) const;

  // Allows editing the calibration data at certain points
  void editCalibrationData(/*data*/);

  // Gets the current calibration status
  CalibrationStatus getStatus() const;

private:
  std::unique_ptr<ICalibrationStrategy> calibrationStrategy;
  CalibrationStatus status;
  IIOHandler *_io;
  IDiagnostics *_diag;
};
