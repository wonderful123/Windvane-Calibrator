/**
 * @file WindVane.cpp
 * @brief Main library initialization and global functions
 */

#include "WindVane.h"
#include <memory>

namespace WindVane {

// Global library state
static bool libraryInitialized = false;
static Config globalConfig;

bool begin(const Config& config) {
    globalConfig = config;
    libraryInitialized = true;
    return true;
}

void end() {
    libraryInitialized = false;
}

const char* getVersion() {
    return "1.0.0";
}

const char* getBuildInfo() {
    return "WindVane Library v1.0.0 - Self-contained wind vane calibration library\n"
           "Build Date: " __DATE__ " " __TIME__ "\n"
           "Platform: Multi-platform (Arduino, ESP32, Host)\n"
           "Features: SOLID principles, Google C++ Style Guide, Comprehensive testing";
}

bool isInitialized() {
    return libraryInitialized;
}

const Config& getGlobalConfig() {
    return globalConfig;
}

} // namespace WindVane