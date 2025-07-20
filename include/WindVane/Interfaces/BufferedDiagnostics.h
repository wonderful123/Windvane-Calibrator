#pragma once
#include "IDiagnosticsSink.h"
#include "IDiagnostics.h"
#include <UI/IIO.h>
#include <deque>
#include <string>

class BufferedDiagnostics final : public IDiagnosticsSink, public IBufferedDiagnostics {
public:
    explicit BufferedDiagnostics(size_t maxEntries = 10, IOutput* out = nullptr)
        : _maxEntries(maxEntries), _out(out) {}

    void handle(const DiagnosticsEvent& ev) override {
        std::string lvl = ev.level == LogLevel::Info ? "INFO" : "WARN";
        std::string msg = "[" + std::to_string(platform::toEmbedded(ev.timestamp)) + "] " + lvl + ": " + ev.message;
        if (_out)
            _out->writeln(msg.c_str());
        push(msg);
    }

    const std::deque<std::string>& history() const override { return _messages; }

    void clear() override { _messages.clear(); }

private:
    size_t _maxEntries;
    std::deque<std::string> _messages;
    IOutput* _out;

    void push(const std::string& m) {
        if (_messages.size() >= _maxEntries)
            _messages.pop_front();
        _messages.push_back(m);
    }
};
