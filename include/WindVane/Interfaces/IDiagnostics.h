#pragma once
#include <deque>
#include <string>

class IDiagnostics {
public:
    virtual ~IDiagnostics() = default;
    virtual void info(const char* msg) = 0;
    virtual void warn(const char* msg) = 0;
};

class IBufferedDiagnostics {
public:
    virtual ~IBufferedDiagnostics() = default;
    virtual const std::deque<std::string>& history() const = 0;
    virtual void clear() = 0;
};
