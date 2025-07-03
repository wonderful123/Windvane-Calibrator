#pragma once

enum class CalibrationStrategyType {
  Spinning,
  // add others here
};

class ICalibrationStrategy {
 public:
  virtual ~ICalibrationStrategy() = default;
  virtual void calibrate() = 0;
  virtual float mapReading(float reading) const = 0;
  virtual CalibrationStrategyType strategyType() const = 0;
};
