#pragma once
#include "IIO.h"
#include <Platform/TimeUtils.h>
#ifdef UNIT_TEST
#include <ArduinoFake.h>
#else
#include <Arduino.h>
#endif

class SerialIOHandler final : public IUserIO {
public:
    bool hasInput() const override { return Serial.available(); }
    char readInput() const override { return Serial.read(); }
    void flushInput() const override {
        while (Serial.available()) Serial.read();
    }
    void waitMs(platform::TimeMs ms) const override {
        delay(platform::toEmbedded(ms));
    }
    bool yesNoPrompt(const char* prompt) const override {
        Serial.println(prompt);
        while (!Serial.available()) delay(10);
        char c = Serial.read();
        flushInput();
        return c == 'y' || c == 'Y';
    }

    float readFloat() const override {
        while (!Serial.available()) delay(10);
        float v = Serial.parseFloat();
        Serial.readStringUntil('\n');
        return v;
    }

    int readInt() const override {
        while (!Serial.available()) delay(10);
        int v = Serial.parseInt();
        Serial.readStringUntil('\n');
        return v;
    }
};

