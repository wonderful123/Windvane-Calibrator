#pragma once

class INumericReader {
public:
    virtual ~INumericReader() = default;
    virtual float readFloat() = 0;
    virtual int readInt() = 0;
};

class IIOHandler {
public:
    virtual ~IIOHandler() = default;
    virtual bool hasInput() = 0;
    virtual char readInput() = 0;
    virtual void flushInput() = 0;
    virtual void waitMs(int ms) = 0;
    virtual bool yesNoPrompt(const char* prompt) = 0;
};

class IUserIO : public IIOHandler, public INumericReader {
public:
    ~IUserIO() override = default;
};

class IOutput {
public:
    virtual ~IOutput() = default;
    virtual void write(const char* text) = 0;
    virtual void writeln(const char* text) = 0;
    virtual void clear() = 0;
};
