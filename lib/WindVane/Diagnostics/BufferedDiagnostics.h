#pragma once
#include "IDiagnosticsSink.h"
#include "IDiagnostics.h"
#include <deque>
#include <string>
#ifdef ARDUINO
#include <Arduino.h>
#endif

class BufferedDiagnostics : public IDiagnosticsSink, public IBufferedDiagnostics {
public:
    explicit BufferedDiagnostics(size_t maxEntries = 10)
        : _maxEntries(maxEntries) {}

    void handle(const DiagnosticsEvent& ev) override {
#ifdef ARDUINO
        const char* lvl = ev.level == LogLevel::Info ? "INFO" : "WARN";
        char buf[128];
        sprintf(buf, "[%lu] %s: %s", platform::toEmbedded(ev.timestamp), lvl, ev.message.c_str());
        Serial.println(buf);
        push(buf);
#else
        std::string lvl = ev.level == LogLevel::Info ? "INFO" : "WARN";
        push("[" + std::to_string(platform::toEmbedded(ev.timestamp)) + "] " + lvl + ": " + ev.message);
#endif
    }

    const std::deque<std::string>& history() const override { return _messages; }

    void clear() override { _messages.clear(); }

private:
    size_t _maxEntries;
    std::deque<std::string> _messages;

    void push(const std::string& m) {
        if (_messages.size() >= _maxEntries)
            _messages.pop_front();
        _messages.push_back(m);
    }
};
