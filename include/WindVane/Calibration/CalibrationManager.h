#pragma once
#include "Calibration/Strategies/ICalibrationStrategy.h"
#include "CalibrationResult.h"
#include <memory>

class CalibrationManager {
public:
  enum class CalibrationStatus {
    NotStarted,
    AwaitingStart,
    InProgress,
    Completed
  };

  explicit CalibrationManager(std::unique_ptr<ICalibrationStrategy> strategy);

  // Prepares the device for calibration and waits for user confirmation
  CalibrationResult beginCalibration();

  // Runs the full calibration process in a single step
  CalibrationResult runCalibration();

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
};
