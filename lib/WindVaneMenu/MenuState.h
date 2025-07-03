#pragma once
#include <string>
#include <vector>
#include <Platform/TimeUtils.h>
#include "MenuTypes.h"

struct MenuState {
    std::vector<PersistedMenuState> stack{PersistedMenuState::Main};
    platform::TimeMs lastActivity{platform::TimeMs{0}};
    platform::TimeMs lastCalibration{platform::TimeMs{0}};
    std::string statusMsg{};
    MenuStatusLevel statusLevel{MenuStatusLevel::Normal};
    platform::TimeMs msgExpiry{platform::TimeMs{0}};
    platform::TimeMs lastDisplayUpdate{platform::TimeMs{0}};
};
