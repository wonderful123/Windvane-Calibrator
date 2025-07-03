#pragma once
#include "IIO.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif
class SerialOutput : public IOutput {
public:
    void write(const char* text) override {
#ifdef ARDUINO
        Serial.print(text);
#else
        (void)text;
#endif
    }
    void writeln(const char* text) override {
#ifdef ARDUINO
        Serial.println(text);
#else
        (void)text;
#endif
    }
    void clear() override {
#ifdef ARDUINO
        Serial.print("\033[2J\033[H");
#endif
    }
};
