#pragma once
#include "IPlatform.h"
#include <WindVaneMenu/WindVaneMenuPresenter.h>

#ifdef ARDUINO
#include <Diagnostics/BasicDiagnostics.h>
#include <UI/SerialIOHandler.h>
#include <UI/SerialOutput.h>

class ArduinoPlatform : public IPlatform {
public:
    platform::TimeMs millis() const override { return ::millis(); }
    void renderStatusLine(WindVaneMenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) const override {
        presenter.renderStatusLine(st, statusStr, msg, level, supportsColor());
    }
    bool supportsColor() const override { return false; }
};
using Platform = ArduinoPlatform;
using PlatformDiagnostics = BasicDiagnostics;
using PlatformIOHandler = SerialIOHandler;
using PlatformOutput = SerialOutput;
#else
#include <Diagnostics/BasicDiagnostics.h>
#include <UI/ConsoleIOHandler.h>
#include <UI/ConsoleOutput.h>
#include <chrono>

class HostPlatform : public IPlatform {
public:
    platform::TimeMs millis() const override {
        using namespace std::chrono;
        static auto start = steady_clock::now();
        return duration_cast<milliseconds>(steady_clock::now() - start);
    }
    void renderStatusLine(WindVaneMenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) const override {
        presenter.renderStatusLine(st, statusStr, msg, level, supportsColor());
    }
    bool supportsColor() const override { return true; }
};
using Platform = HostPlatform;
using PlatformDiagnostics = BasicDiagnostics;
using PlatformIOHandler = ConsoleIOHandler;
using PlatformOutput = ConsoleOutput;
#endif
