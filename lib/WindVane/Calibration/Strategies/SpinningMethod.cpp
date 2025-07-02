#include "Calibration/Strategies/SpinningMethod.h"
#include "../../IADC.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#else
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>
#include <numeric>
#endif

using namespace std::chrono_literals;

// ---------------------- Debouncer implementation ---------------------
SpinningMethod::Debouncer::Debouncer(size_t size, int majority)
    : _buffer(size, 0.0f), _index(0), _majority(majority), _filled(false) {}

void SpinningMethod::Debouncer::addSample(float value) {
  _buffer[_index] = value;
  _index = (_index + 1) % _buffer.size();
  if (_index == 0)
    _filled = true;
}

bool SpinningMethod::Debouncer::full() const { return _filled; }

float SpinningMethod::Debouncer::average() const {
  float sum = std::accumulate(_buffer.begin(), _buffer.end(), 0.0f);
  return sum / (_filled ? _buffer.size() : _index);
}

float SpinningMethod::Debouncer::noise() const {
  size_t count = _filled ? _buffer.size() : _index;
  if (count == 0)
    return 0.0f;
  float avg = average();
  float sum = 0.0f;
  for (size_t i = 0; i < count; ++i) {
    float diff = _buffer[i] - avg;
    sum += diff * diff;
  }
  return std::sqrt(sum / count);
}

bool SpinningMethod::Debouncer::stable(float tolerance) const {
  size_t count = _filled ? _buffer.size() : _index;
  if (count == 0)
    return false;
  float last = _buffer[((_index == 0 ? count : _index) - 1) % count];
  int within = 0;
  for (size_t i = 0; i < count; ++i) {
    if (std::fabs(_buffer[i] - last) < tolerance)
      ++within;
  }
  return within >= _majority;
}

// ---------------------- SpinningMethod ---------------------

SpinningMethod::SpinningMethod(IADC *adc) : _adc(adc) {}

float SpinningMethod::adaptiveThreshold() const {
  if (_positions.size() < 2)
    return 0.05f;

  std::vector<float> values;
  values.reserve(_positions.size());
  for (const auto &p : _positions)
    values.push_back(p.value);
  std::sort(values.begin(), values.end());
  float sum = 0.0f;
  for (size_t i = 1; i < values.size(); ++i)
    sum += values[i] - values[i - 1];
  sum += 1.0f - values.back() + values.front();
  float avgSep = sum / values.size();
  float noise = _debouncer.noise();
  return std::max(noise * 2.0f, avgSep * 0.5f);
}

bool SpinningMethod::isNewPosition(float reading, float threshold) const {
  for (const auto &pos : _positions) {
    if (std::fabs(reading - pos.value) < threshold)
      return false;
  }
  return true;
}

bool SpinningMethod::isAnomaly(float reading, float last) const {
  if (reading < 0.0f || reading > 1.0f)
    return true;
  if (std::fabs(reading - last) > 0.2f)
    return true;
  return false;
}

void SpinningMethod::addOrUpdatePosition(float reading, float threshold) {
  for (auto &pos : _positions) {
    if (std::fabs(reading - pos.value) < threshold) {
      ++pos.count;
      pos.value += (reading - pos.value) / pos.count;
      pos.min = std::min(pos.min, reading);
      pos.max = std::max(pos.max, reading);
      return;
    }
  }
  _positions.push_back({reading, reading, reading, 1});
}

void SpinningMethod::saveCalibration() const {
#ifdef ARDUINO
  EEPROM.begin(_positions.size() * sizeof(Position));
  for (size_t i = 0; i < _positions.size(); ++i) {
    EEPROM.put(i * sizeof(Position), _positions[i]);
  }
  EEPROM.commit();
#else
  std::ofstream ofs("calibration.dat");
  for (const auto &p : _positions) {
    ofs << p.value << ',' << p.min << ',' << p.max << ',' << p.count << "\n";
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

  const std::chrono::milliseconds sampleDelay(10);

  float last = _adc->read();
  _positions.clear();
  _debouncer = Debouncer();
  _debouncer.addSample(last);

  size_t lastPositionCount = 0;
  int rotationsWithoutNew = 0;
  bool stop = false;

  while (!stop) {
    float reading = _adc->read();

    if (isAnomaly(reading, last)) {
      ++_anomalyCount;
      continue;
    }

    _debouncer.addSample(reading);
    float threshold = adaptiveThreshold();

    if (_debouncer.stable(threshold)) {
      bool newPos = isNewPosition(reading, threshold);
      addOrUpdatePosition(reading, threshold);
      if (newPos) {
        rotationsWithoutNew = 0;
        lastPositionCount = _positions.size();
#ifdef ARDUINO
        Serial.print(F("Position detected: "));
        Serial.println(_positions.size());
#else
        std::cout << "Position detected: " << _positions.size() << std::endl;
#endif
      } else if (!_positions.empty() &&
                 std::fabs(reading - _positions.front().value) < threshold) {
        if (lastPositionCount == _positions.size()) {
          ++rotationsWithoutNew;
        } else {
          rotationsWithoutNew = 0;
          lastPositionCount = _positions.size();
        }
      }
      last = reading;
    }

    float certainty = std::min(rotationsWithoutNew / 2.0f, 1.0f) * 100.0f;
#ifdef ARDUINO
    Serial.print(F("Certainty: "));
    Serial.print(certainty, 1);
    Serial.println(F("%"));
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 's' || c == 'S')
        stop = true;
    }
    delay(sampleDelay.count());
#else
    std::cout << "Certainty: " << certainty << "%" << std::endl;
    if (std::cin.rdbuf()->in_avail()) {
      char c;
      std::cin.get(c);
      if (c == 's' || c == 'S')
        stop = true;
    }
    std::this_thread::sleep_for(sampleDelay);
#endif

    if (rotationsWithoutNew >= 3)
      stop = true;
    last = reading;
  }

#ifdef ARDUINO
  Serial.println(F("Calibration stopped."));
#else
  std::cout << "Calibration stopped." << std::endl;
#endif

  saveCalibration();
}
