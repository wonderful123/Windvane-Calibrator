#include "Calibration/Strategies/SpinningMethod.h"
#include "../../IADC.h"
#include "../../Storage/ICalibrationStorage.h"
#include "../../Diagnostics/IDiagnostics.h"
#include <chrono>
#include <cmath>
#include <algorithm>
#include <thread>
#include <string>

using namespace std::chrono_literals;


SpinningMethod::SpinningMethod(const SpinningMethodDeps &deps)
    : _adc(deps.adc), _storage(deps.storage),
      _diag(deps.diag), _config(deps.config) {
  if (_storage) {
    int version = 0;
    std::vector<ClusterData> clusters;
    if (_storage->load(clusters, version))
      _clusterMgr.setClusters(clusters);
  }
}



void SpinningMethod::saveCalibration() const {
  if (_storage)
    _storage->save(_clusterMgr.clusters(), CALIBRATION_VERSION);
}

void SpinningMethod::calibrate() {
  SessionState state;
  initSession(state);

  const std::chrono::milliseconds sampleDelay(_config.sampleDelayMs);
  const std::chrono::seconds stallTimeout(_config.stallTimeoutSec);

  while (!state.stop) {
    float reading = _adc->read();
    if (checkStall(std::chrono::steady_clock::now(), state.lastIncrease,
                   stallTimeout))
      state.stop = true;

    processReading(reading, state);
    std::this_thread::sleep_for(sampleDelay);
  }

  finalizeCalibration(state.abort, _config.threshold * 1.5f);
}

float SpinningMethod::mapReading(float reading) const {
  return _clusterMgr.interpolate(reading);
}

bool SpinningMethod::checkStall(std::chrono::steady_clock::time_point now,
                                std::chrono::steady_clock::time_point &last,
                                const std::chrono::seconds &timeout) const {
  if (now - last > timeout) {
    return true;
  }
  return false;
}

void SpinningMethod::updateClusters(float reading, SessionState &state) {
  _recent.push_back(reading);
  if (_recent.size() > static_cast<size_t>(_config.bufferSize))
    _recent.pop_front();

  int inRange = 0;
  for (float r : _recent) {
    if (std::fabs(r - reading) < _config.threshold)
      ++inRange;
  }

  if (inRange > static_cast<int>(_recent.size()) / 2) {
    bool added = _clusterMgr.addOrUpdate(reading, _config.threshold);
    if (_clusterMgr.clusters().size() != state.previousCount) {
      std::string msg = "Position detected: " +
                        std::to_string(_clusterMgr.clusters().size()) + "/" +
                        std::to_string(_config.expectedPositions);
      _diag->info(msg.c_str());
      state.previousCount = _clusterMgr.clusters().size();
      state.lastIncrease = std::chrono::steady_clock::now();
      if (_clusterMgr.clusters().size() >= static_cast<size_t>(_config.expectedPositions)) {
        state.stop = true;
      }
    }
    (void)added;
  }
}

void SpinningMethod::finalizeCalibration(bool abort, float mergeThreshold) {
  _diag->info("Calibration stopped.");

  _clusterMgr.mergeAndPrune(mergeThreshold, 2);
  if (!abort) {
    _clusterMgr.diagnostics(*_diag);
    saveCalibration();
  } else {
    _diag->info("Calibration aborted. Previous data preserved.");
  }
}

void SpinningMethod::initSession(SessionState &state) {
  _clusterMgr.clear();
  _recent.clear();
  state = SessionState{}; // reset fields
}

void SpinningMethod::processReading(float reading, SessionState &state) {
  if (reading <= 0.0f || reading >= 1.0f) {
    _clusterMgr.recordAnomaly();
    return;
  }

  updateClusters(reading, state);

  if (state.prevReading >= 0 && reading < state.prevReading)
    _diag->warn("Warning: reverse rotation detected");
  state.prevReading = reading;
}
