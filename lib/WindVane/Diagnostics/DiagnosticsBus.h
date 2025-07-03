#pragma once
#include "IDiagnostics.h"
#include "IDiagnosticsSink.h"
#include <algorithm>
#include <vector>

class DiagnosticsBus : public IDiagnostics {
public:
    void addSink(IDiagnosticsSink* sink) { if (sink) _sinks.push_back(sink); }
    void removeSink(IDiagnosticsSink* sink) {
        _sinks.erase(std::remove(_sinks.begin(), _sinks.end(), sink), _sinks.end());
    }

    void info(const char* msg) override { dispatch(LogLevel::Info, msg); }
    void warn(const char* msg) override { dispatch(LogLevel::Warn, msg); }

private:
    std::vector<IDiagnosticsSink*> _sinks;

    void dispatch(LogLevel level, const char* msg) {
        DiagnosticsEvent ev{level, platform::now(), std::string(msg)};
        for (auto* s : _sinks) if (s) s->handle(ev);
    }
};
