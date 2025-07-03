#pragma once

static inline const char* compassPoint(float deg) {
    static const char* pts[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    int idx = static_cast<int>((deg + 22.5f) / 45.0f) & 7;
    return pts[idx];
}
