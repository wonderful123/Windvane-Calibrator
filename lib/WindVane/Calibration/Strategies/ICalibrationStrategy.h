#pragma once

class ICalibrationStrategy {
public:
  virtual ~ICalibrationStrategy() = default;

  // Starts the Calibration process of the wind vane.
  virtual void calibrate() = 0;
};
