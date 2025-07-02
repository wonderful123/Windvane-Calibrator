#include "Calibration/Strategies/SpinningMethod.h"
#include "../../IADC.h"
#include "../../Storage/ICalibrationStorage.h"

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

SpinningMethod::SpinningMethod(IADC *adc, ICalibrationStorage *storage)
    : _adc(adc), _storage(storage) {}


bool SpinningMethod::addOrUpdateCluster(float reading, float threshold) {
  for (auto &cluster : _clusters) {
    if (std::fabs(reading - cluster.mean) < threshold) {
      cluster.mean = (cluster.mean * cluster.count + reading) / (cluster.count + 1);
      cluster.min = std::min(cluster.min, reading);
      cluster.max = std::max(cluster.max, reading);
      ++cluster.count;
      return false;
    }
  }
  _clusters.push_back({reading, reading, reading, 1});
  return true;
}

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
#ifdef ARDUINO
  Serial.println(
      F("Align vane to forward reference and press any key to start."));
  while (!_io.hasInput())
    _io.waitMs(10);
  _io.flushInput();
#else
  std::cout << "Align vane to forward reference and press ENTER to start."
            << std::endl;
  std::cin.get();
#endif

  const float threshold = 0.05f;      // sensor difference threshold
  const int bufferSize = 5;            // samples in circular buffer
  const int expectedPositions = 16;    // typical reed switch count
  const std::chrono::milliseconds sampleDelay(10);

  _clusters.clear();
  _recent.clear();
  _anomalyCount = 0;
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
#ifdef ARDUINO
      if (_io.yesNoPrompt(F("No new positions detected for a while. Stop and save calibration? (Y/N)")))
        stop = true;
      else
        lastIncrease = now;
#else
      if (_io.yesNoPrompt("No new positions detected for a while. Stop and save calibration? (Y/N)"))
        stop = true;
      else
        lastIncrease = now;
#endif
    }

    if (reading <= 0.0f || reading >= 1.0f) {
      ++_anomalyCount;
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
        bool added = addOrUpdateCluster(reading, threshold);
        if (_clusters.size() != previousCount) {
#ifdef ARDUINO
          Serial.print(F("Position detected: "));
          Serial.print(_clusters.size());
          Serial.print(F("/"));
          Serial.print(expectedPositions);
          Serial.print(F(" ("));
          Serial.print(static_cast<int>(100.0f * _clusters.size() /
                                         expectedPositions));
          Serial.println(F("% complete)"));
          if (_clusters.size() > 1) {
            float separation =
                _clusters.back().mean - _clusters[_clusters.size() - 2].mean;
            if (separation < threshold * 2) {
              Serial.println(F("Warning: positions very close"));
            }
          }
#else
          std::cout << "Position detected: " << _clusters.size() << "/"
                    << expectedPositions << " ("
                    << static_cast<int>(100.0f * _clusters.size() /
                                      expectedPositions)
                    << "% complete)" << std::endl;
          if (_clusters.size() > 1) {
            float separation =
                _clusters.back().mean - _clusters[_clusters.size() - 2].mean;
            if (separation < threshold * 2) {
              std::cout << "Warning: positions very close" << std::endl;
            }
          }
#endif
          previousCount = _clusters.size();
          lastIncrease = std::chrono::steady_clock::now();
          if (_clusters.size() >= static_cast<size_t>(expectedPositions)) {
#ifdef ARDUINO
            if (_io.yesNoPrompt(F("All expected positions detected. Stop now? (Y/N)")))
              stop = true;
#else
            if (_io.yesNoPrompt("All expected positions detected. Stop now? (Y/N)"))
              stop = true;
#endif
          }
        }

        (void)added; // suppress unused warning if above block disabled
      }

      if (prevReading >= 0 && reading < prevReading) {
#ifdef ARDUINO
        Serial.println(F("Warning: reverse rotation detected"));
#else
        std::cout << "Warning: reverse rotation detected" << std::endl;
#endif
      }
      prevReading = reading;
    }

    if (_io.hasInput()) {
      char c = _io.readInput();
      if (c == 's' || c == 'S') {
        bool confirm = true;
        if (_clusters.size() != static_cast<size_t>(expectedPositions)) {
#ifdef ARDUINO
          confirm = _io.yesNoPrompt(F("Calibration incomplete. Stop anyway? (Y/N)"));
#else
          confirm = _io.yesNoPrompt("Calibration incomplete. Stop anyway? (Y/N)");
#endif
        }
        if (confirm)
          stop = true;
      } else if (c == 'q' || c == 'Q') {
        abort = true;
        stop = true;
      }
    }
    _io.waitMs(sampleDelay.count());
  }

#ifdef ARDUINO
  Serial.println(F("Calibration stopped."));
#else
  std::cout << "Calibration stopped." << std::endl;
#endif

  mergeAndPruneClusters(threshold * 1.5f, 2);
  if (!abort)
    saveCalibration();
  else {
#ifdef ARDUINO
    Serial.println(F("Calibration aborted. Previous data preserved."));
#else
    std::cout << "Calibration aborted. Previous data preserved." << std::endl;
#endif
  }
  if (!abort)
    printDiagnostics();
}

void SpinningMethod::printDiagnostics() const {
#ifdef ARDUINO
  Serial.print(F("Anomalies detected: ")); Serial.println(_anomalyCount);
  for (size_t i = 0; i < _clusters.size(); ++i) {
    Serial.print(F("Cluster ")); Serial.print(i);
    Serial.print(F(": mean=")); Serial.print(_clusters[i].mean, 4);
    Serial.print(F(" min=")); Serial.print(_clusters[i].min, 4);
    Serial.print(F(" max=")); Serial.print(_clusters[i].max, 4);
    Serial.print(F(" count=")); Serial.print(_clusters[i].count);
    float gap = 0;
    if (i + 1 < _clusters.size())
      gap = _clusters[i + 1].mean - _clusters[i].mean;
    Serial.print(F(" gap=")); Serial.println(gap, 4);
  }
  if (_clusters.size() > 1) {
    float expectedGap = 1.0f / _clusters.size();
    for (size_t i = 0; i + 1 < _clusters.size(); ++i) {
      float gap = _clusters[i + 1].mean - _clusters[i].mean;
      if (gap < expectedGap * 0.5f)
        Serial.println(F("Warning: clusters too close"));
      if (gap > expectedGap * 1.5f)
        Serial.println(F("Warning: clusters too far apart"));
    }
  }
#else
  std::cout << "Anomalies detected: " << _anomalyCount << std::endl;
  for (size_t i = 0; i < _clusters.size(); ++i) {
    float gap = 0;
    if (i + 1 < _clusters.size())
      gap = _clusters[i + 1].mean - _clusters[i].mean;
    std::cout << "Cluster " << i << ": mean=" << _clusters[i].mean
              << " min=" << _clusters[i].min
              << " max=" << _clusters[i].max
              << " count=" << _clusters[i].count
              << " gap=" << gap << std::endl;
  }
  if (_clusters.size() > 1) {
    float expectedGap = 1.0f / _clusters.size();
    for (size_t i = 0; i + 1 < _clusters.size(); ++i) {
      float gap = _clusters[i + 1].mean - _clusters[i].mean;
      if (gap < expectedGap * 0.5f)
        std::cout << "Warning: clusters " << i << " and " << i + 1
                  << " very close" << std::endl;
      if (gap > expectedGap * 1.5f)
        std::cout << "Warning: clusters " << i << " and " << i + 1
                  << " far apart" << std::endl;
    }
  }
#endif
}
