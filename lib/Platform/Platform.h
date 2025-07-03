#pragma once
#include "IPlatform.h"
#include <WindVaneMenu/MenuPresenter.h>

#ifdef ARDUINO
#include <Diagnostics/BasicDiagnostics.h>
#include <Diagnostics/DiagnosticsBus.h>
#include <UI/SerialIOHandler.h>
#include <UI/SerialOutput.h>

class ArduinoPlatform : public IPlatform {
public:
    platform::TimeMs millis() const override { return platform::TimeMs{::millis()}; }
    void renderStatusLine(MenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) const override {
        presenter.renderStatusLine(st, statusStr, msg, level, supportsColor());
    }
    bool supportsColor() const override { return false; }
};
using Platform = ArduinoPlatform;
using PlatformDiagnostics = DiagnosticsBus;
using PlatformDiagnosticsSink = BasicDiagnostics;
using PlatformIOHandler = SerialIOHandler;
using PlatformOutput = SerialOutput;
#else
#include <Diagnostics/BasicDiagnostics.h>
#include <Diagnostics/DiagnosticsBus.h>
#include <UI/ConsoleIOHandler.h>
#include <UI/ConsoleOutput.h>
#include <chrono>

class HostPlatform : public IPlatform {
public:
    platform::TimeMs millis() const override {
        using namespace std::chrono;
        static auto start = steady_clock::now();
        return platform::TimeMs{static_cast<platform::TimeMs::rep>(duration_cast<milliseconds>(steady_clock::now() - start).count())};
    }
    void renderStatusLine(MenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) const override {
        presenter.renderStatusLine(st, statusStr, msg, level, supportsColor());
    }
    bool supportsColor() const override { return true; }
};
using Platform = HostPlatform;
using PlatformDiagnostics = DiagnosticsBus;
using PlatformDiagnosticsSink = BasicDiagnostics;
using PlatformIOHandler = ConsoleIOHandler;
using PlatformOutput = ConsoleOutput;
#endif
