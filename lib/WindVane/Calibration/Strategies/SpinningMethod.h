#pragma once
#include "ICalibrationStrategy.h"
#include <vector>
#include <cstdint>
#include <array>
#include <optional>

class IADC;

// Implements a spinning calibration strategy that records unique positions
// while the user rotates the vane.
class SpinningMethod : public ICalibrationStrategy {
public:
  explicit SpinningMethod(IADC *adc);

  // Runs the interactive calibration procedure.
  void calibrate() override;

private:
  struct Position {
    float value;
    float min;
    float max;
    int count;
  };

  class Debouncer {
  public:
    Debouncer(size_t size = 5, int majority = 3);
    void addSample(float value);
    bool stable(float tolerance) const;
    float average() const;
    float noise() const;
    bool full() const;

  private:
    std::vector<float> _buffer;
    size_t _index{0};
    int _majority{3};
    bool _filled{false};
  };

  IADC *_adc;
  std::vector<Position> _positions;
  Debouncer _debouncer;
  int _anomalyCount{0};
  size_t _rotationsWithoutNew{0};

  bool isNewPosition(float reading, float threshold) const;
  void addOrUpdatePosition(float reading, float threshold);
  float adaptiveThreshold() const;
  bool isAnomaly(float reading, float last) const;
  void saveCalibration() const;
};
