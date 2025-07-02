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
  if (_positions.empty())
    return true;
  return std::fabs(reading - _positions.back()) > threshold;
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
  Serial.println(F("Align the vane to forward reference then spin."));
#else
  std::cout << "Align the vane to forward reference then spin." << std::endl;
#endif

  const float threshold = 0.05f;           // sensor difference threshold
  const int stableSamples = 5;             // debounce count
  const std::chrono::milliseconds sampleDelay(10);

  float last = _adc->read();
  int stableCount = 0;
  _positions.clear();

  while (true) {
    float reading = _adc->read();
    if (std::fabs(reading - last) < threshold) {
      ++stableCount;
    } else {
      stableCount = 0;
      last = reading;
    }

    if (stableCount >= stableSamples && isNewPosition(reading, threshold)) {
      _positions.push_back(reading);
#ifdef ARDUINO
      Serial.print(F("Position detected: "));
      Serial.println(_positions.size());
#else
      std::cout << "Position detected: " << _positions.size() << std::endl;
#endif

      // consider calibration complete after returning to first position
      if (_positions.size() > 1 &&
          std::fabs(reading - _positions.front()) < threshold) {
        break;
      }
    }

#ifdef ARDUINO
    delay(sampleDelay.count());
#else
    std::this_thread::sleep_for(sampleDelay);
#endif
  }

#ifdef ARDUINO
  Serial.println(F("Calibration complete."));
#else
  std::cout << "Calibration complete." << std::endl;
#endif

  saveCalibration();
}

