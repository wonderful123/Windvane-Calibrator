#pragma once
#include <deque>
#include <string>

class IBufferedDiagnostics {
public:
    virtual ~IBufferedDiagnostics() = default;
    virtual const std::deque<std::string>& history() const = 0;
    virtual void clear() = 0;
};
