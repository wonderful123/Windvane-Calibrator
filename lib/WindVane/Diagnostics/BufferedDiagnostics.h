#pragma once
#include "IDiagnostics.h"
#include <deque>
#include <string>
#ifdef ARDUINO
#include <Arduino.h>
#endif

class BufferedDiagnostics : public IDiagnostics, public IBufferedDiagnostics {
public:
    explicit BufferedDiagnostics(size_t maxEntries = 10)
        : _maxEntries(maxEntries) {}

    void info(const char* msg) override {
#ifdef ARDUINO
        Serial.println(msg);
#endif
        push("INFO: " + std::string(msg));
    }

    void warn(const char* msg) override {
#ifdef ARDUINO
        Serial.println(msg);
#endif
        push("WARN: " + std::string(msg));
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
