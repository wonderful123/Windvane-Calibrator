#pragma once
#include "IDiagnostics.h"
#include <UI/IIO.h>

class BasicDiagnostics : public IDiagnostics {
public:
    explicit BasicDiagnostics(IOutput* out) : _out(out) {}
    void info(const char* msg) override { if (_out) _out->writeln(msg); }
    void warn(const char* msg) override { if (_out) _out->writeln(msg); }
private:
    IOutput* _out;
};

