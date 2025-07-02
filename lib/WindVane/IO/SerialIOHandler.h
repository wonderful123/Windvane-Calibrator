#pragma once
#include "IIOHandler.h"
#ifdef ARDUINO
#include <Arduino.h>
#endif

class SerialIOHandler : public IIOHandler {
public:
    bool hasInput() override {
#ifdef ARDUINO
        return Serial.available();
#else
        return false;
#endif
    }
    char readInput() override {
#ifdef ARDUINO
        return Serial.read();
#else
        return 0;
#endif
    }
    void flushInput() override {
#ifdef ARDUINO
        while (Serial.available())
            Serial.read();
#endif
    }
    void waitMs(int ms) override {
#ifdef ARDUINO
        delay(ms);
#else
        (void)ms;
#endif
    }
    bool yesNoPrompt(const char* prompt) override {
#ifdef ARDUINO
        Serial.println(prompt);
        while (!Serial.available())
            delay(10);
        char c = Serial.read();
        flushInput();
        return c == 'y' || c == 'Y';
#else
        (void)prompt;
        return false;
#endif
    }
};

