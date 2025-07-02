#pragma once
#include "ICalibrationStrategy.h"
#include <vector>
#include <cstdint>

class IADC;

// Implements a spinning calibration strategy that records unique positions
// while the user rotates the vane.
class SpinningMethod : public ICalibrationStrategy {
public:
  explicit SpinningMethod(IADC *adc);

  // Runs the interactive calibration procedure.
  void calibrate() override;

private:
  IADC *_adc;
  std::vector<float> _positions;

  bool isNewPosition(float reading, float threshold) const;
  void saveCalibration() const;
};