#pragma once
#include "Calibration/Strategies/ICalibrationStrategy.h"

class CalibrationManager {
public:
  enum class CalibrationStatus {
    NotStarted,
    AwaitingStart,
    InProgress,
    Completed
  };

  CalibrationManager(ICalibrationStrategy *strategy);

  // Starts the calibration process and sets status to AwaitingStart
  void startCalibration();

  // Begins the actual calibration based on a received signal
  void beginCalibration();

  // Ends the calibration and sets status to Completed
  void endCalibration();

  // Gets the current calibration data
  void getCalibratedData(float rawWindDirection);

  // Allows editing the calibration data at certain points
  void editCalibrationData(/*data*/);

  // Gets the current calibration status
  CalibrationStatus getStatus() const;

private:
  ICalibrationStrategy *calibrationStrategy;
  CalibrationStatus status;
};