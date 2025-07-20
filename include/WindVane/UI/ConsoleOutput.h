#pragma once
#include "IIO.h"
#include <iostream>
class ConsoleOutput final : public IOutput {
public:
    void write(const char* text) const override { std::cout << text; }
    void writeln(const char* text) const override { std::cout << text << std::endl; }
    void clear() const override { std::cout << "\033[2J\033[H"; }
};
