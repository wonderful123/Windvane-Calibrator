#pragma once
#include "IDiagnosticsSink.h"
#include <iostream>

class ConsoleDiagnostics : public IDiagnosticsSink {
public:
    void handle(const DiagnosticsEvent& ev) override {
        const char* lvl = ev.level == LogLevel::Info ? "INFO" : "WARN";
        std::cout << '[' << platform::toEmbedded(ev.timestamp) << "] " << lvl << ": " << ev.message << std::endl;
    }
};
