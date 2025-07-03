#pragma once
class IOutput {
public:
    virtual ~IOutput() = default;
    virtual void write(const char* text) = 0;
    virtual void writeln(const char* text) = 0;
    virtual void clear() = 0;
};
