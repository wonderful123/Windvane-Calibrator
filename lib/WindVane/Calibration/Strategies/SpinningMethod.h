#pragma once
#include "ICalibrationStrategy.h"
#include <deque>
#include <vector>
#include <cstdint>
#include <chrono>
#include "../ClusterData.h"
#include "../ClusterManager.h"
#include "../../Storage/ICalibrationStorage.h"
#include "../../IO/IIOHandler.h"
#include "../../Diagnostics/IDiagnostics.h"

class IADC;

// Implements a spinning calibration strategy that records unique positions
// while the user rotates the vane.
struct SpinningConfig {
  float threshold = 0.05f;      ///< Minimum delta to consider a new position
  int bufferSize = 5;           ///< Number of samples for noise filtering
  int expectedPositions = 16;   ///< Expected number of detent positions
  int sampleDelayMs = 10;       ///< Delay between samples
  int stallTimeoutSec = 5;      ///< Seconds without new detections before prompt
};

class SpinningMethod : public ICalibrationStrategy {
public:
  SpinningMethod(IADC *adc, ICalibrationStorage *storage,
                 IIOHandler *io, IDiagnostics *diag,
                 SpinningConfig config = {});

  // Runs the interactive calibration procedure.
  void calibrate() override;

  // Map a raw ADC reading to a calibrated direction in degrees
  float mapReading(float reading) const override;

  const SpinningConfig& config() const { return _config; }
  void setConfig(const SpinningConfig& cfg) { _config = cfg; }

  static constexpr int CALIBRATION_VERSION = 1;

private:
  IADC *_adc;
  ICalibrationStorage *_storage;
  IIOHandler *_io;
  IDiagnostics *_diag;
  ClusterManager _clusterMgr;
  std::deque<float> _recent;
  SpinningConfig _config;

  void saveCalibration() const;

  void promptStart() const;
  bool checkStall(std::chrono::steady_clock::time_point now,
                  std::chrono::steady_clock::time_point &last,
                  const std::chrono::seconds &timeout) const;
  void updateClusters(float reading, float threshold, int bufferSize,
                      int expectedPositions, size_t &prevCount,
                      std::chrono::steady_clock::time_point &lastIncrease,
                      bool &stop);
  void handleUserCommand(bool &stop, bool &abort, int expectedPositions);
  void finalizeCalibration(bool abort, float mergeThreshold);
};
