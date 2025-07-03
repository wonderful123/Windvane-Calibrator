#pragma once
#include "Calibration/Strategies/ICalibrationStrategy.h"
#include "../UI/IIO.h"
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
                     IUserIO *io, IDiagnostics *diag);

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

  ICalibrationStrategy* strategy() const { return calibrationStrategy.get(); }

private:
  std::unique_ptr<ICalibrationStrategy> calibrationStrategy;
  CalibrationStatus status;
  IUserIO *_io;
  IDiagnostics *_diag;

  void promptUserStart() const;
  void finishCalibrationMessage() const;
};
