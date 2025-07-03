#pragma once
#include "IOutput.h"
#include <iostream>
class ConsoleOutput : public IOutput {
public:
    void write(const char* text) override { std::cout << text; }
    void writeln(const char* text) override { std::cout << text << std::endl; }
    void clear() override { std::cout << "\033[2J\033[H"; }
};
