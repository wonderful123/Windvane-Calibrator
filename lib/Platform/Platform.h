#pragma once

#ifdef ARDUINO
#include <Diagnostics/SerialDiagnostics.h>
#include <IO/SerialIOHandler.h>
#include <IO/SerialOutput.h>
using PlatformDiagnostics = SerialDiagnostics;
using PlatformIOHandler = SerialIOHandler;
using PlatformOutput = SerialOutput;
inline unsigned long platformMillis() { return millis(); }
#define PLATFORM_RENDER_STATUSLINE(p, st, statusStr, msg, level) \
    p.renderStatusLineArduino(st, statusStr, msg, level)
#else
#include <Diagnostics/ConsoleDiagnostics.h>
#include <IO/ConsoleIOHandler.h>
#include <IO/ConsoleOutput.h>
#include <chrono>
using PlatformDiagnostics = ConsoleDiagnostics;
using PlatformIOHandler = ConsoleIOHandler;
using PlatformOutput = ConsoleOutput;
inline unsigned long platformMillis() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - start).count();
}
#define PLATFORM_RENDER_STATUSLINE(p, st, statusStr, msg, level) \
    p.renderStatusLineHost(st, statusStr, msg, level)
#endif
