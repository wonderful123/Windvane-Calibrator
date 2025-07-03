#pragma once
#include <chrono>
#include <cstdint>

namespace platform {

struct TimeMs {
    using rep = uint32_t;
    rep value;

    constexpr TimeMs() : value(0) {}
    explicit constexpr TimeMs(rep v) : value(v) {}
    explicit constexpr TimeMs(std::chrono::milliseconds d)
        : value(static_cast<rep>(d.count())) {}

    constexpr rep count() const { return value; }

    constexpr TimeMs operator+(TimeMs other) const {
        return TimeMs{static_cast<rep>(value + other.value)};
    }
    constexpr TimeMs operator-(TimeMs other) const {
        return TimeMs{static_cast<rep>(value - other.value)};
    }
    TimeMs& operator+=(TimeMs other) {
        value = static_cast<rep>(value + other.value);
        return *this;
    }
    TimeMs& operator-=(TimeMs other) {
        value = static_cast<rep>(value - other.value);
        return *this;
    }

    friend constexpr bool operator==(TimeMs a, TimeMs b) { return a.value == b.value; }
    friend constexpr bool operator!=(TimeMs a, TimeMs b) { return a.value != b.value; }
    friend constexpr bool operator<(TimeMs a, TimeMs b) { return a.value < b.value; }
    friend constexpr bool operator>(TimeMs a, TimeMs b) { return a.value > b.value; }
    friend constexpr bool operator<=(TimeMs a, TimeMs b) { return a.value <= b.value; }
    friend constexpr bool operator>=(TimeMs a, TimeMs b) { return a.value >= b.value; }
};

inline TimeMs add(TimeMs a, TimeMs b) { return a + b; }
inline TimeMs elapsed(TimeMs start, TimeMs end) { return end - start; }

TimeMs now();

inline uint32_t toEmbedded(TimeMs t) { return t.count(); }
inline std::chrono::milliseconds toChrono(TimeMs t) { return std::chrono::milliseconds(t.count()); }
inline TimeMs fromChrono(std::chrono::milliseconds d) { return TimeMs{static_cast<TimeMs::rep>(d.count())}; }

} // namespace platform
