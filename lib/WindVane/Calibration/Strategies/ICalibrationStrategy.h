#pragma once

class ICalibrationStrategy {
public:
  virtual ~ICalibrationStrategy() = default;

  // Starts the Calibration process of the wind vane.
  virtual void calibrate() = 0;

  // Convert a raw ADC reading to a calibrated direction in degrees
  virtual float mapReading(float reading) const = 0;
};
