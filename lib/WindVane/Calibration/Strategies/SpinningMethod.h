#pragma once
#include "ICalibrationStrategy.h"

class SpinningMethod : public ICalibrationStrategy {
public:
  virtual void calibrate() override;
};