#pragma once

class ICalibrationStrategy {
public:
  // Starts the Calibration process of the wind vane.
  virtual void calibrate() = 0;
};