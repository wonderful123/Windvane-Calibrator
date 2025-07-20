#pragma once

enum class CalibrationStrategyType {
  Spinning,
  // add others here
};

#include "../CalibrationConfig.h"

class ICalibrationStrategy {
 public:
  virtual ~ICalibrationStrategy() = default;
  virtual void calibrate() = 0;
  virtual float mapReading(float reading) const = 0;
  virtual CalibrationStrategyType strategyType() const = 0;
  virtual void setConfig(const CalibrationConfig& cfg) = 0;
  virtual CalibrationConfig config() const = 0;
};
