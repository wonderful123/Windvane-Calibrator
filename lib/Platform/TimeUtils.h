#pragma once
#include <chrono>

namespace platform {
#ifdef ARDUINO
using TimeMs = unsigned long;
inline TimeMs toEmbedded(std::chrono::milliseconds d) { return static_cast<TimeMs>(d.count()); }
inline std::chrono::milliseconds toChrono(TimeMs ms) { return std::chrono::milliseconds(ms); }
#else
using TimeMs = std::chrono::milliseconds;
inline TimeMs toEmbedded(std::chrono::milliseconds d) { return d; }
inline std::chrono::milliseconds toChrono(TimeMs d) { return d; }
#endif
}
