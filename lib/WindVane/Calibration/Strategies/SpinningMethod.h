#pragma once
#include "ICalibrationStrategy.h"
#include <deque>
#include <vector>
#include <cstdint>
#include <chrono>
#include "../ClusterData.h"
#include "../ClusterManager.h"
#include "../SpinningConfig.h"
#include "../CalibrationConfig.h"
#include "../../Storage/ICalibrationStorage.h"
#include "../../Diagnostics/IDiagnostics.h"

class IADC;

// Implements a spinning calibration strategy that records unique positions
// while the user rotates the vane.

struct SpinningMethodDeps {
  IADC *adc{};
  ICalibrationStorage *storage{};
  IDiagnostics *diag{};
  SpinningConfig config{};
};

class SpinningMethod : public ICalibrationStrategy {
public:
  explicit SpinningMethod(const SpinningMethodDeps &deps);

  CalibrationStrategyType strategyType() const override {
    return CalibrationStrategyType::Spinning;
  }

  // Runs the interactive calibration procedure.
  void calibrate() override;

  // Map a raw ADC reading to a calibrated direction in degrees
  float mapReading(float reading) const override;

  CalibrationConfig config() const override {
    CalibrationConfig cfg;
    cfg.spin = _config;
    return cfg;
  }
  void setConfig(const CalibrationConfig& cfg) override { _config = cfg.spin; }

  static constexpr int CALIBRATION_VERSION = 1;

private:
  IADC *_adc;
  ICalibrationStorage *_storage;
  IDiagnostics *_diag;
  ClusterManager _clusterMgr;
  std::deque<float> _recent;
  SpinningConfig _config;

  struct SessionState {
    size_t previousCount{0};
    bool stop{false};
    bool abort{false};
    float prevReading{-1.0f};
    std::chrono::steady_clock::time_point lastIncrease{std::chrono::steady_clock::now()};
  };

  void saveCalibration() const;

  bool checkStall(std::chrono::steady_clock::time_point now,
                  std::chrono::steady_clock::time_point &last,
                  const std::chrono::seconds &timeout) const;
  void updateClusters(float reading, SessionState &state);
  void finalizeCalibration(bool abort, float mergeThreshold);
  void runSession(SessionState &state, std::chrono::milliseconds delay,
                  std::chrono::seconds timeout);
  void processReading(float reading, SessionState &state);
  void initSession(SessionState &state);
};
