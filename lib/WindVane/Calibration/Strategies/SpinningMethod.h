#pragma once
#include "ICalibrationStrategy.h"
#include <deque>
#include <vector>
#include <cstdint>
#include "../ClusterData.h"
#include "../ClusterManager.h"
#include "../../Storage/ICalibrationStorage.h"
#include "../../IO/IIOHandler.h"
#include "../../Diagnostics/IDiagnostics.h"

class IADC;

// Implements a spinning calibration strategy that records unique positions
// while the user rotates the vane.
class SpinningMethod : public ICalibrationStrategy {
public:
  SpinningMethod(IADC *adc, ICalibrationStorage *storage,
                 IIOHandler *io, IDiagnostics *diag);

  // Runs the interactive calibration procedure.
  void calibrate() override;

  // Map a raw ADC reading to a calibrated direction in degrees
  float mapReading(float reading) const override;

  static constexpr int CALIBRATION_VERSION = 1;

private:
  IADC *_adc;
  ICalibrationStorage *_storage;
  IIOHandler *_io;
  IDiagnostics *_diag;
  ClusterManager _clusterMgr;
  std::deque<float> _recent;

  void saveCalibration() const;
};
