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

  static constexpr int CALIBRATION_VERSION = 1;

private:
  class IOHandler {
  public:
    bool hasInput() const;
    char readInput() const;
    void flushInput() const;
    void waitMs(int ms) const;
#ifdef ARDUINO
    bool yesNoPrompt(const __FlashStringHelper *prompt) const;
#else
    bool yesNoPrompt(const char *prompt) const;
#endif
  };

  struct PositionCluster {
    float mean;
    float min;
    float max;
    int count;
  };

  IADC *_adc;
  IOHandler _io;
  std::vector<PositionCluster> _clusters;
  std::deque<float> _recent;
  int _anomalyCount{0};

  void mergeAndPruneClusters(float mergeThreshold, int minCount);

  bool addOrUpdateCluster(float reading, float threshold);
  void saveCalibration() const;
  void printDiagnostics() const;
};
