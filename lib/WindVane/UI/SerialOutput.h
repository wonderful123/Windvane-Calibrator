#pragma once
#include "IIO.h"
#ifdef UNIT_TEST
#include <ArduinoFake.h>
#else
#include <Arduino.h>
#endif
class SerialOutput final : public IOutput {
public:
    void write(const char* text) const override { Serial.print(text); }
    void writeln(const char* text) const override { Serial.println(text); }
    void clear() const override { Serial.print("\033[2J\033[H"); }
};
