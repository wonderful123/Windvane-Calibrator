#pragma once
#include "IIO.h"
#include <Platform/TimeUtils.h>
#ifdef ARDUINO
#include <Arduino.h>
#endif

class SerialIOHandler : public IUserIO {
public:
    bool hasInput() const override {
#ifdef ARDUINO
        return Serial.available();
#else
        return false;
#endif
    }
    char readInput() const override {
#ifdef ARDUINO
        return Serial.read();
#else
        return 0;
#endif
    }
    void flushInput() const override {
#ifdef ARDUINO
        while (Serial.available())
            Serial.read();
#endif
    }
    void waitMs(platform::TimeMs ms) const override {
#ifdef ARDUINO
        delay(platform::toEmbedded(ms));
#else
        (void)ms;
#endif
    }
    bool yesNoPrompt(const char* prompt) const override {
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

    float readFloat() const override {
#ifdef ARDUINO
        while (!Serial.available())
            delay(10);
        float v = Serial.parseFloat();
        Serial.readStringUntil('\n');
        return v;
#else
        return 0.0f;
#endif
    }

    int readInt() const override {
#ifdef ARDUINO
        while (!Serial.available())
            delay(10);
        int v = Serial.parseInt();
        Serial.readStringUntil('\n');
        return v;
#else
        return 0;
#endif
    }
};

