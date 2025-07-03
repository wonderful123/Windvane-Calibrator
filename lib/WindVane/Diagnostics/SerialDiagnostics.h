#pragma once
#include "IDiagnosticsSink.h"
#ifdef UNIT_TEST
#include <ArduinoFake.h>
#else
#include <Arduino.h>
#endif

class SerialDiagnostics : public IDiagnosticsSink {
public:
    void handle(const DiagnosticsEvent& ev) override {
        const char* lvl = ev.level == LogLevel::Info ? "INFO" : "WARN";
        char buf[128];
        sprintf(buf, "[%lu] %s: %s", platform::toEmbedded(ev.timestamp), lvl, ev.message.c_str());
        Serial.println(buf);
    }
};
