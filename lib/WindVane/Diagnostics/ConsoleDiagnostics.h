#pragma once
#include "IDiagnostics.h"
#include <iostream>

class ConsoleDiagnostics : public IDiagnostics {
public:
    void info(const char* msg) override { std::cout << msg << std::endl; }
    void warn(const char* msg) override { std::cout << msg << std::endl; }
};

