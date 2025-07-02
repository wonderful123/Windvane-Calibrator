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

  // Starts the calibration process and sets status to AwaitingStart
  bool startCalibration();

  // Begins the actual calibration based on a received signal
  bool beginCalibration();

  // Ends the calibration and sets status to Completed
  bool endCalibration();

  // Gets the current calibration data
  void getCalibratedData(float rawWindDirection);

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
