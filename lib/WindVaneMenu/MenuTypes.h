#pragma once
#include <string>

enum class MenuStatusLevel { Normal, Warning, Error };

enum class PersistedMenuState {
    Main = 0,
    LiveDisplay,
    Calibrate,
    Diagnostics,
    Settings,
    Help
};

struct MenuResult {
    bool success{false};
    std::string message{};
};
