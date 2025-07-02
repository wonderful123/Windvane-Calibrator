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


#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#else
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#endif

using namespace std::chrono_literals;

bool SpinningMethod::IOHandler::hasInput() const {
#ifdef ARDUINO
  return Serial.available();
#else
  return std::cin.rdbuf()->in_avail();
#endif
}

char SpinningMethod::IOHandler::readInput() const {
#ifdef ARDUINO
  return Serial.read();
#else
  char c;
  std::cin.get(c);
  return c;
#endif
}

void SpinningMethod::IOHandler::flushInput() const {
#ifdef ARDUINO
  while (Serial.available())
    Serial.read();
#endif
}

void SpinningMethod::IOHandler::waitMs(int ms) const {
#ifdef ARDUINO
  delay(ms);
#else
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

#ifdef ARDUINO
bool SpinningMethod::IOHandler::yesNoPrompt(const __FlashStringHelper *prompt) const {
  Serial.println(prompt);
  while (!Serial.available())
    delay(10);
  char c = Serial.read();
  flushInput();
  return c == 'y' || c == 'Y';
}
#else
bool SpinningMethod::IOHandler::yesNoPrompt(const char *prompt) const {
  std::cout << prompt << std::endl;
  char c;
  std::cin >> c;
  return c == 'y' || c == 'Y';
}
#endif

SpinningMethod::SpinningMethod(IADC *adc, ICalibrationStorage *storage,
                               IIOHandler *io, IDiagnostics *diag)
    : _adc(adc), _storage(storage), _io(io), _diag(diag) {}

void SpinningMethod::mergeAndPruneClusters(float mergeThreshold, int minCount) {
  if (_clusters.empty())
    return;
  std::sort(_clusters.begin(), _clusters.end(),
            [](const ClusterData &a, const ClusterData &b) {
              return a.mean < b.mean;
            });
  std::vector<ClusterData> merged;
  size_t i = 0;
  while (i < _clusters.size()) {
    ClusterData cluster = _clusters[i];
    size_t j = i + 1;
    while (j < _clusters.size() &&
           std::fabs(_clusters[j].mean - cluster.mean) < mergeThreshold) {
      float total = cluster.count + _clusters[j].count;
      cluster.mean =
          (cluster.mean * cluster.count + _clusters[j].mean * _clusters[j].count) /
          total;
      cluster.min = std::min(cluster.min, _clusters[j].min);
      cluster.max = std::max(cluster.max, _clusters[j].max);
      cluster.count += _clusters[j].count;
      ++j;
    }
    if (cluster.count >= minCount)
      merged.push_back(cluster);
    i = j;
  }
  _clusters = std::move(merged);
}

void SpinningMethod::saveCalibration() const {
  if (_storage)
    _storage->save(_clusters, CALIBRATION_VERSION);
}

void SpinningMethod::calibrate() {
  _diag->info("Align vane to forward reference and press any key to start.");
  while (!_io->hasInput())
    _io->waitMs(10);
  _io->flushInput();

  const float threshold = 0.05f;
  const int bufferSize = 5;
  const int expectedPositions = 16;
  const std::chrono::milliseconds sampleDelay(10);

  _clusterMgr.clear();
  _recent.clear();
  size_t previousCount = 0;
  bool stop = false;
  bool abort = false;
  float prevReading = -1.0f;
  auto lastIncrease = std::chrono::steady_clock::now();
  const std::chrono::seconds stallTimeout(5);

  while (!stop) {
    float reading = _adc->read();

    auto now = std::chrono::steady_clock::now();
    if (now - lastIncrease > stallTimeout) {
      if (_io->yesNoPrompt("No new positions detected for a while. Stop and save calibration? (Y/N)"))
        stop = true;
      else
        lastIncrease = now;
    }

    if (reading <= 0.0f || reading >= 1.0f) {
      _clusterMgr.recordAnomaly();
    } else {
      _recent.push_back(reading);
      if (_recent.size() > bufferSize)
        _recent.pop_front();

      int inRange = 0;
      for (float r : _recent) {
        if (std::fabs(r - reading) < threshold)
          ++inRange;
      }

      if (inRange > static_cast<int>(_recent.size()) / 2) {
        bool added = _clusterMgr.addOrUpdate(reading, threshold);
        if (_clusterMgr.clusters().size() != previousCount) {
          std::string msg = "Position detected: " +
                            std::to_string(_clusterMgr.clusters().size()) + "/" +
                            std::to_string(expectedPositions);
          _diag->info(msg.c_str());
          previousCount = _clusterMgr.clusters().size();
          lastIncrease = std::chrono::steady_clock::now();
          if (_clusterMgr.clusters().size() >= static_cast<size_t>(expectedPositions)) {
            if (_io->yesNoPrompt("All expected positions detected. Stop now? (Y/N)"))
              stop = true;
          }
        }
        (void)added;
      }

      if (prevReading >= 0 && reading < prevReading) {
        _diag->warn("Warning: reverse rotation detected");
      }
      prevReading = reading;
    }

    if (_io->hasInput()) {
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
    _io->waitMs(sampleDelay.count());
  }

  _diag->info("Calibration stopped.");

  _clusterMgr.mergeAndPrune(threshold * 1.5f, 2);
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
