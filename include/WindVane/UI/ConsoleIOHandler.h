#pragma once
#include "IIO.h"
#include <Platform/TimeUtils.h>
#include <chrono>
#include <iostream>
#include <limits>
#include <thread>

class ConsoleIOHandler final : public IUserIO {
public:
    bool hasInput() const override {
        return std::cin.rdbuf()->in_avail();
    }
    char readInput() const override {
        char c;
        std::cin.get(c);
        return c;
    }
    void flushInput() const override {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    void waitMs(platform::TimeMs ms) const override {
        std::this_thread::sleep_for(platform::toChrono(ms));
    }
    bool yesNoPrompt(const char* prompt) const override {
        std::cout << prompt << std::endl;
        char c;
        std::cin >> c;
        flushInput();
        return c == 'y' || c == 'Y';
    }

    float readFloat() const override {
        float v = 0.0f;
        if (std::cin >> v) {
            flushInput();
        } else {
            // Clear error state and flush input
            std::cin.clear();
            flushInput();
        }
        return v;
    }

    int readInt() const override {
        int v = 0;
        if (std::cin >> v) {
            flushInput();
        } else {
            // Clear error state and flush input
            std::cin.clear();
            flushInput();
        }
        return v;
    }
};

