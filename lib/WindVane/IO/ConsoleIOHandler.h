#pragma once
#include "IUserIO.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <thread>

class ConsoleIOHandler : public IUserIO {
public:
    bool hasInput() override {
        return std::cin.rdbuf()->in_avail();
    }
    char readInput() override {
        char c;
        std::cin.get(c);
        return c;
    }
    void flushInput() override {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    void waitMs(int ms) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    bool yesNoPrompt(const char* prompt) override {
        std::cout << prompt << std::endl;
        char c;
        std::cin >> c;
        flushInput();
        return c == 'y' || c == 'Y';
    }

    float readFloat() override {
        float v;
        std::cin >> v;
        flushInput();
        return v;
    }

    int readInt() override {
        int v;
        std::cin >> v;
        flushInput();
        return v;
    }
};

