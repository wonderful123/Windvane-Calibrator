#include "Calibration/Strategies/SpinningMethod.h"
#include "../../IADC.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#else
#include <chrono>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <thread>
#endif

using namespace std::chrono_literals;

SpinningMethod::SpinningMethod(IADC *adc) : _adc(adc) {}


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

void SpinningMethod::saveCalibration() const {
#ifdef ARDUINO
  EEPROM.begin(_clusters.size() * sizeof(float));
  for (size_t i = 0; i < _clusters.size(); ++i) {
    EEPROM.put(i * sizeof(float), _clusters[i].mean);
  }
  EEPROM.commit();
#else
  std::ofstream ofs("calibration.dat");
  for (size_t i = 0; i < _clusters.size(); ++i) {
    ofs << _clusters[i].mean << "\n";
  }
#endif
}

void SpinningMethod::calibrate() {
#ifdef ARDUINO
  Serial.println(
      F("Align vane to forward reference and press any key to start."));
  while (!Serial.available()) {
    delay(10);
  }
  while (Serial.available())
    Serial.read();
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
  float prevReading = -1.0f;

  while (!stop) {
    float reading = _adc->read();

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
          Serial.println(expectedPositions);
          if (_clusters.size() > 1) {
            float separation =
                _clusters.back().mean - _clusters[_clusters.size() - 2].mean;
            if (separation < threshold * 2) {
              Serial.println(F("Warning: positions very close"));
            }
          }
#else
          std::cout << "Position detected: " << _clusters.size() << "/"
                    << expectedPositions << std::endl;
          if (_clusters.size() > 1) {
            float separation =
                _clusters.back().mean - _clusters[_clusters.size() - 2].mean;
            if (separation < threshold * 2) {
              std::cout << "Warning: positions very close" << std::endl;
            }
          }
#endif
          previousCount = _clusters.size();
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

#ifdef ARDUINO
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 's' || c == 'S')
        stop = true;
    }
    delay(sampleDelay.count());
#else
    if (std::cin.rdbuf()->in_avail()) {
      char c;
      std::cin.get(c);
      if (c == 's' || c == 'S')
        stop = true;
    }
    std::this_thread::sleep_for(sampleDelay);
#endif
  }

#ifdef ARDUINO
  Serial.println(F("Calibration stopped."));
#else
  std::cout << "Calibration stopped." << std::endl;
#endif

  std::sort(_clusters.begin(), _clusters.end(),
            [](const PositionCluster &a, const PositionCluster &b) {
              return a.mean < b.mean;
            });
  saveCalibration();
  printDiagnostics();
}

void SpinningMethod::printDiagnostics() const {
#ifdef ARDUINO
  Serial.print(F("Anomalies detected: ")); Serial.println(_anomalyCount);
  for (size_t i = 0; i < _clusters.size(); ++i) {
    Serial.print(F("Cluster ")); Serial.print(i);
    Serial.print(F(": mean=")); Serial.print(_clusters[i].mean, 4);
    Serial.print(F(" min=")); Serial.print(_clusters[i].min, 4);
    Serial.print(F(" max=")); Serial.println(_clusters[i].max, 4);
  }
#else
  std::cout << "Anomalies detected: " << _anomalyCount << std::endl;
  for (size_t i = 0; i < _clusters.size(); ++i) {
    std::cout << "Cluster " << i << ": mean=" << _clusters[i].mean
              << " min=" << _clusters[i].min
              << " max=" << _clusters[i].max << std::endl;
  }
#endif
}
