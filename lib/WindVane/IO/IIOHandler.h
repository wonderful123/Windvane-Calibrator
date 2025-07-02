#pragma once
class IIOHandler {
public:
    virtual ~IIOHandler() = default;
    virtual bool hasInput() = 0;
    virtual char readInput() = 0;
    virtual void flushInput() = 0;
    virtual void waitMs(int ms) = 0;
    virtual bool yesNoPrompt(const char* prompt) = 0;
};

