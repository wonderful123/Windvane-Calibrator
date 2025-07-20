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
        // Add timeout to prevent infinite waiting
        unsigned long startTime = millis();
        while (!Serial.available() && (millis() - startTime < 30000)) delay(10);
        if (!Serial.available()) return 0.0f; // Timeout
        float v = Serial.parseFloat();
        Serial.readStringUntil('\n');
        return v;
    }

    int readInt() const override {
        // Add timeout to prevent infinite waiting
        unsigned long startTime = millis();
        while (!Serial.available() && (millis() - startTime < 30000)) delay(10);
        if (!Serial.available()) return 0; // Timeout
        int v = Serial.parseInt();
        Serial.readStringUntil('\n');
        return v;
    }
};

