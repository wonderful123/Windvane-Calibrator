#pragma once

class IUserIO {
public:
    virtual ~IUserIO() = default;
    virtual bool hasInput() = 0;
    virtual char readInput() = 0;
    virtual void flushInput() = 0;
    virtual void waitMs(int ms) = 0;
    virtual bool yesNoPrompt(const char* prompt) = 0;
    // Optional numeric helpers
    virtual float readFloat() { return 0.0f; }
    virtual int readInt() { return 0; }
};

class IOutput {
public:
    virtual ~IOutput() = default;
    virtual void write(const char* text) = 0;
    virtual void writeln(const char* text) = 0;
    virtual void clear() = 0;
};
