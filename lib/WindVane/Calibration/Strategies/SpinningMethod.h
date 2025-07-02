#pragma once
#include "ICalibrationStrategy.h"
#include <deque>
#include <vector>
#include <cstdint>
#include "../ClusterData.h"
#include "../../Storage/ICalibrationStorage.h"

class IADC;

// Implements a spinning calibration strategy that records unique positions
// while the user rotates the vane.
class SpinningMethod : public ICalibrationStrategy {
public:
  SpinningMethod(IADC *adc, ICalibrationStorage *storage);

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

  IADC *_adc;
  ICalibrationStorage *_storage;
  IOHandler _io;
  std::vector<ClusterData> _clusters;
  std::deque<float> _recent;
  int _anomalyCount{0};

  void mergeAndPruneClusters(float mergeThreshold, int minCount);

  bool addOrUpdateCluster(float reading, float threshold);
  void saveCalibration() const;
  void printDiagnostics() const;
};
