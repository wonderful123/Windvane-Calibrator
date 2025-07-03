#pragma once
#include <Platform/TimeUtils.h>

class IUserIO {
public:
    virtual ~IUserIO() = default;
    virtual bool hasInput() const = 0;
    virtual char readInput() const = 0;
    virtual void flushInput() const = 0;
    virtual void waitMs(platform::TimeMs ms) const = 0;
    virtual bool yesNoPrompt(const char* prompt) const = 0;
    // Optional numeric helpers
    virtual float readFloat() const { return 0.0f; }
    virtual int readInt() const { return 0; }
};

class IOutput {
public:
    virtual ~IOutput() = default;
    virtual void write(const char* text) const = 0;
    virtual void writeln(const char* text) const = 0;
    virtual void clear() const = 0;
};
