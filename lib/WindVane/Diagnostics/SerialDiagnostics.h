#pragma once
#include "IDiagnostics.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif

class SerialDiagnostics : public IDiagnostics {
public:
    void info(const char* msg) override {
#ifdef ARDUINO
        Serial.println(msg);
#endif
    }
    void warn(const char* msg) override {
#ifdef ARDUINO
        Serial.println(msg);
#endif
    }
};

