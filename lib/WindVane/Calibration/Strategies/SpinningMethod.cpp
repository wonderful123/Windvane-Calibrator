#include "Calibration/Strategies/SpinningMethod.h"
#include "../../IADC.h"
#include "../../Storage/ICalibrationStorage.h"
#include "../../IO/IIOHandler.h"
#include "../../Diagnostics/IDiagnostics.h"
#include <chrono>
#include <cmath>
#include <algorithm>
#include <thread>
#include <string>

using namespace std::chrono_literals;


SpinningMethod::SpinningMethod(IADC *adc, ICalibrationStorage *storage,
                               IIOHandler *io, IDiagnostics *diag,
                               SpinningConfig config)
    : _adc(adc), _storage(storage), _io(io), _diag(diag), _config(config) {
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
  promptStart();

  const float threshold = _config.threshold;
  const int bufferSize = _config.bufferSize;
  const int expectedPositions = _config.expectedPositions;
  const std::chrono::milliseconds sampleDelay(_config.sampleDelayMs);

  _clusterMgr.clear();
  _recent.clear();
  size_t previousCount = 0;
  bool stop = false;
  bool abort = false;
  float prevReading = -1.0f;
  auto lastIncrease = std::chrono::steady_clock::now();
  const std::chrono::seconds stallTimeout(_config.stallTimeoutSec);

  while (!stop) {
    float reading = _adc->read();
    if (checkStall(std::chrono::steady_clock::now(), lastIncrease, stallTimeout))
      stop = true;

    if (reading <= 0.0f || reading >= 1.0f) {
      _clusterMgr.recordAnomaly();
    } else {
      updateClusters(reading, threshold, bufferSize, expectedPositions,
                    previousCount, lastIncrease, stop);

      if (prevReading >= 0 && reading < prevReading)
        _diag->warn("Warning: reverse rotation detected");
      prevReading = reading;
    }

    handleUserCommand(stop, abort, expectedPositions);
    _io->waitMs(sampleDelay.count());
  }

  finalizeCalibration(abort, threshold * 1.5f);
}

float SpinningMethod::mapReading(float reading) const {
  return _clusterMgr.interpolate(reading);
}

void SpinningMethod::promptStart() const {
  _diag->info("Align vane to forward reference and press any key to start.");
  while (!_io->hasInput())
    _io->waitMs(10);
  _io->flushInput();
}

bool SpinningMethod::checkStall(std::chrono::steady_clock::time_point now,
                                std::chrono::steady_clock::time_point &last,
                                const std::chrono::seconds &timeout) const {
  if (now - last > timeout) {
    if (_io->yesNoPrompt("No new positions detected for a while. Stop and save calibration? (Y/N)"))
      return true;
    last = now;
  }
  return false;
}

void SpinningMethod::updateClusters(float reading, float threshold, int bufferSize,
                                    int expectedPositions, size_t &prevCount,
                                    std::chrono::steady_clock::time_point &lastIncrease,
                                    bool &stop) {
  _recent.push_back(reading);
  if (_recent.size() > static_cast<size_t>(bufferSize))
    _recent.pop_front();

  int inRange = 0;
  for (float r : _recent) {
    if (std::fabs(r - reading) < threshold)
      ++inRange;
  }

  if (inRange > static_cast<int>(_recent.size()) / 2) {
    bool added = _clusterMgr.addOrUpdate(reading, threshold);
    if (_clusterMgr.clusters().size() != prevCount) {
      std::string msg = "Position detected: " +
                        std::to_string(_clusterMgr.clusters().size()) + "/" +
                        std::to_string(expectedPositions);
      _diag->info(msg.c_str());
      prevCount = _clusterMgr.clusters().size();
      lastIncrease = std::chrono::steady_clock::now();
      if (_clusterMgr.clusters().size() >= static_cast<size_t>(expectedPositions)) {
        if (_io->yesNoPrompt("All expected positions detected. Stop now? (Y/N)"))
          stop = true;
      }
    }
    (void)added;
  }
}

void SpinningMethod::handleUserCommand(bool &stop, bool &abort, int expectedPositions) {
  if (!_io->hasInput())
    return;
  char c = _io->readInput();
  if (c == 's' || c == 'S') {
    bool confirm = true;
    if (_clusterMgr.clusters().size() != static_cast<size_t>(expectedPositions))
      confirm = _io->yesNoPrompt("Calibration incomplete. Stop anyway? (Y/N)");
    if (confirm)
      stop = true;
  } else if (c == 'q' || c == 'Q') {
    abort = true;
    stop = true;
  }
}

void SpinningMethod::finalizeCalibration(bool abort, float mergeThreshold) {
  _diag->info("Calibration stopped.");

  _clusterMgr.mergeAndPrune(mergeThreshold, 2);
  if (!abort) {
    _clusterMgr.diagnostics(*_diag);
    if (_io->yesNoPrompt("Save calibration results? (Y/N)"))
      saveCalibration();
    else
      _diag->info("Calibration discarded at user request.");
  } else {
    _diag->info("Calibration aborted. Previous data preserved.");
  }
}
