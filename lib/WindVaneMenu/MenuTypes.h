#pragma once
#include <string>

enum class MenuStatusLevel { Normal, Warning, Error };

struct MenuResult {
    bool success{false};
    std::string message{};
};
