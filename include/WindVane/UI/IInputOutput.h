#pragma once
#include <Platform/TimeUtils.h>

// Focused interfaces following Interface Segregation Principle

class ICharacterInput {
public:
    virtual ~ICharacterInput() = default;
    virtual bool hasInput() const = 0;
    virtual char readInput() const = 0;
    virtual void flushInput() const = 0;
};

class INumericInput {
public:
    virtual ~INumericInput() = default;
    virtual float readFloat() const = 0;
    virtual int readInt() const = 0;
};

class IUserInteraction {
public:
    virtual ~IUserInteraction() = default;
    virtual bool yesNoPrompt(const char* prompt) const = 0;
};

class ITimingControl {
public:
    virtual ~ITimingControl() = default;
    virtual void waitMs(platform::TimeMs ms) const = 0;
};

class IOutput {
public:
    virtual ~IOutput() = default;
    virtual void write(const char* text) const = 0;
    virtual void writeln(const char* text) const = 0;
    virtual void clear() const = 0;
};

// Composite interface for backward compatibility
class IUserIO : public ICharacterInput, 
                public INumericInput, 
                public IUserInteraction, 
                public ITimingControl {
public:
    virtual ~IUserIO() = default;
};