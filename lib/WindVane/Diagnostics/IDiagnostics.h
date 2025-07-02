#pragma once

class IDiagnostics {
public:
    virtual ~IDiagnostics() = default;
    virtual void info(const char* msg) = 0;
    virtual void warn(const char* msg) = 0;
};

