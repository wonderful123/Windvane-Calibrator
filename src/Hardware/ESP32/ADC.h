#pragma once

#include "WindVane/IADC.h"

class ESP32ADC : public IADC {
public:
  ESP32ADC(int GPIOPin) : _pin(GPIOPin) {}

  int getPin() const { return _pin; }

  float read() override;

private:
  int _pin;
};