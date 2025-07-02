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
#endif

using namespace std::chrono_literals;

SpinningMethod::SpinningMethod(IADC *adc) : _adc(adc) {}

bool SpinningMethod::isNewPosition(float reading, float threshold) const {
  for (float pos : _positions) {
    if (std::fabs(reading - pos) < threshold)
      return false;
  }
  return true;
}

void SpinningMethod::saveCalibration() const {
#ifdef ARDUINO
  EEPROM.begin(_positions.size() * sizeof(float));
  for (size_t i = 0; i < _positions.size(); ++i) {
    EEPROM.put(i * sizeof(float), _positions[i]);
  }
  EEPROM.commit();
#else
  std::ofstream ofs("calibration.dat");
  for (size_t i = 0; i < _positions.size(); ++i) {
    ofs << _positions[i] << "\n";
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

  const float threshold = 0.05f; // sensor difference threshold
  const int stableSamples = 5;   // debounce count
  const std::chrono::milliseconds sampleDelay(10);

  float last = _adc->read();
  int stableCount = 0;
  _positions.clear();

  size_t lastPositionCount = 0;
  int rotationsWithoutNew = 0;
  bool stop = false;

  while (!stop) {
    float reading = _adc->read();
    if (std::fabs(reading - last) < threshold) {
      ++stableCount;
    } else {
      stableCount = 0;
      last = reading;
    }

    if (stableCount >= stableSamples) {
      if (isNewPosition(reading, threshold)) {
        _positions.push_back(reading);
        rotationsWithoutNew = 0;
        lastPositionCount = _positions.size();
#ifdef ARDUINO
        Serial.print(F("Position detected: "));
        Serial.println(_positions.size());
#else
        std::cout << "Position detected: " << _positions.size() << std::endl;
#endif
      } else if (!_positions.empty() &&
                 std::fabs(reading - _positions.front()) < threshold) {
        if (lastPositionCount == _positions.size()) {
          ++rotationsWithoutNew;
        } else {
          rotationsWithoutNew = 0;
          lastPositionCount = _positions.size();
        }
      }
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
  }

#ifdef ARDUINO
  Serial.println(F("Calibration stopped."));
#else
  std::cout << "Calibration stopped." << std::endl;
#endif

  saveCalibration();
}
