#pragma once
#include "IPlatform.h"
#include <WindVaneMenu/WindVaneMenuPresenter.h>

#ifdef ARDUINO
#include <Diagnostics/SerialDiagnostics.h>
#include <UI/SerialIOHandler.h>
#include <UI/SerialOutput.h>

class ArduinoPlatform : public IPlatform {
public:
    unsigned long millis() override { return ::millis(); }
    void renderStatusLine(WindVaneMenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) override {
        presenter.renderStatusLineArduino(st, statusStr, msg, level);
    }
};
using Platform = ArduinoPlatform;
using PlatformDiagnostics = SerialDiagnostics;
using PlatformIOHandler = SerialIOHandler;
using PlatformOutput = SerialOutput;
#else
#include <Diagnostics/ConsoleDiagnostics.h>
#include <UI/ConsoleIOHandler.h>
#include <UI/ConsoleOutput.h>
#include <chrono>

class HostPlatform : public IPlatform {
public:
    unsigned long millis() override {
        using namespace std::chrono;
        static auto start = steady_clock::now();
        return duration_cast<milliseconds>(steady_clock::now() - start).count();
    }
    void renderStatusLine(WindVaneMenuPresenter& presenter,
                          const WindVaneStatus& st,
                          const char* statusStr,
                          const std::string& msg,
                          MenuStatusLevel level) override {
        presenter.renderStatusLineHost(st, statusStr, msg, level);
    }
};
using Platform = HostPlatform;
using PlatformDiagnostics = ConsoleDiagnostics;
using PlatformIOHandler = ConsoleIOHandler;
using PlatformOutput = ConsoleOutput;
#endif
