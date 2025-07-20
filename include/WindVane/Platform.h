/**
 * @file Platform.h
 * @brief Platform abstraction interfaces for the WindVane library
 */

#pragma once

#include "Types.h"
#include <string>

namespace WindVane {

/**
 * @brief Platform interface for time and system operations
 */
class IPlatform {
public:
    virtual ~IPlatform() = default;
    
    /**
     * @brief Get current time in milliseconds
     * 
     * @return Current time in milliseconds
     */
    virtual TimeMs millis() const = 0;
    
    /**
     * @brief Get platform type
     * 
     * @return Platform type
     */
    virtual PlatformType getPlatformType() const = 0;
    
    /**
     * @brief Get platform name
     * 
     * @return Platform name string
     */
    virtual const char* getPlatformName() const = 0;
    
    /**
     * @brief Check if platform supports color output
     * 
     * @return true if color output is supported
     */
    virtual bool supportsColor() const = 0;
    
    /**
     * @brief Get system memory info
     * 
     * @param freeMemory Output parameter for free memory
     * @param totalMemory Output parameter for total memory
     */
    virtual void getMemoryInfo(size_t& freeMemory, size_t& totalMemory) const = 0;
    
    /**
     * @brief Get system uptime
     * 
     * @return System uptime in milliseconds
     */
    virtual TimeMs getUptime() const = 0;
    
    /**
     * @brief Delay for specified time
     * 
     * @param ms Time to delay in milliseconds
     */
    virtual void delay(TimeMs ms) const = 0;
    
    /**
     * @brief Get random number
     * 
     * @param min Minimum value
     * @param max Maximum value
     * @return Random number between min and max
     */
    virtual uint32_t random(uint32_t min, uint32_t max) const = 0;
};

/**
 * @brief Factory for creating platform instances
 */
class PlatformFactory {
public:
    /**
     * @brief Create platform instance for specified type
     * 
     * @param type Platform type
     * @return Unique pointer to platform instance
     */
    static std::unique_ptr<IPlatform> create(PlatformType type);
    
    /**
     * @brief Create platform instance for current platform
     * 
     * @return Unique pointer to platform instance
     */
    static std::unique_ptr<IPlatform> create();
    
    /**
     * @brief Detect current platform type
     * 
     * @return Detected platform type
     */
    static PlatformType detectPlatform();
    
    /**
     * @brief Get default platform for Arduino
     * 
     * @return Platform instance for Arduino
     */
    static std::unique_ptr<IPlatform> arduino();
    
    /**
     * @brief Get default platform for ESP32
     * 
     * @return Platform instance for ESP32
     */
    static std::unique_ptr<IPlatform> esp32();
    
    /**
     * @brief Get default platform for host
     * 
     * @return Platform instance for host
     */
    static std::unique_ptr<IPlatform> host();
};

/**
 * @brief Time utilities for platform-independent time operations
 */
class TimeUtils {
public:
    /**
     * @brief Get current time
     * 
     * @return Current time in milliseconds
     */
    static TimeMs now();
    
    /**
     * @brief Add two time values
     * 
     * @param a First time value
     * @param b Second time value
     * @return Sum of time values
     */
    static TimeMs add(TimeMs a, TimeMs b);
    
    /**
     * @brief Calculate elapsed time
     * 
     * @param start Start time
     * @param end End time
     * @return Elapsed time
     */
    static TimeMs elapsed(TimeMs start, TimeMs end);
    
    /**
     * @brief Convert time to embedded format
     * 
     * @param t Time value
     * @return Embedded time format
     */
    static uint32_t toEmbedded(TimeMs t);
    
    /**
     * @brief Convert time to chrono format
     * 
     * @param t Time value
     * @return Chrono time format
     */
    static std::chrono::milliseconds toChrono(TimeMs t);
    
    /**
     * @brief Convert from chrono format
     * 
     * @param d Chrono time value
     * @return Time value
     */
    static TimeMs fromChrono(std::chrono::milliseconds d);
};

} // namespace WindVane