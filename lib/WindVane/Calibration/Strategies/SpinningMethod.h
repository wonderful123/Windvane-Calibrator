#pragma once
#include "ICalibrationStrategy.h"
#include <deque>
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
  struct PositionCluster {
    float mean;
    float min;
    float max;
    int count;
  };

  IADC *_adc;
  std::vector<PositionCluster> _clusters;
  std::deque<float> _recent;
  int _anomalyCount{0};

  bool addOrUpdateCluster(float reading, float threshold);
  void saveCalibration() const;
  void printDiagnostics() const;
};