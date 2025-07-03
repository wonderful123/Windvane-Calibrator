#pragma once
#include <string>
#include <Platform/TimeUtils.h>

enum class LogLevel { Info, Warn };

struct DiagnosticsEvent {
    LogLevel level;
    platform::TimeMs timestamp;
    std::string message;
};
