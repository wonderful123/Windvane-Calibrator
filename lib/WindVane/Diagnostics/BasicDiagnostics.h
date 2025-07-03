#pragma once
#include "IDiagnosticsSink.h"
#include <UI/IIO.h>

class BasicDiagnostics final : public IDiagnosticsSink {
public:
    explicit BasicDiagnostics(IOutput* out) : _out(out) {}
    void handle(const DiagnosticsEvent& ev) override {
        if (!_out) return;
        const char* lvl = ev.level == LogLevel::Info ? "INFO" : "WARN";
        char buf[128];
        snprintf(buf, sizeof(buf), "[%lu] %s: %s",
                 platform::toEmbedded(ev.timestamp), lvl, ev.message.c_str());
        _out->writeln(buf);
    }
private:
    IOutput* _out;
};
