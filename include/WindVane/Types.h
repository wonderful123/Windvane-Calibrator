/**
 * @file Types.h
 * @brief Public types and enums for the WindVane library
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace WindVane {

/**
 * @brief Wind vane types supported by the library
 */
enum class VaneType : uint8_t {
    REED_SWITCH = 0,    ///< Reed switch based wind vane
    POTENTIOMETER = 1,  ///< Potentiometer based wind vane
    MAGNETIC = 2,       ///< Magnetic sensor based wind vane
    OPTICAL = 3         ///< Optical encoder based wind vane
};

/**
 * @brief Calibration methods supported by the library
 */
enum class CalibrationMethod : uint8_t {
    SPINNING = 0,       ///< Spinning calibration method
    STATIC = 1,         ///< Static calibration method
    AUTOMATIC = 2,      ///< Automatic calibration method
    MANUAL = 3          ///< Manual calibration method
};

/**
 * @brief Storage types for calibration data
 */
enum class StorageType : uint8_t {
    EEPROM = 0,         ///< EEPROM storage
    FLASH = 1,          ///< Flash memory storage
    SD_CARD = 2,        ///< SD card storage
    FILE_SYSTEM = 3     ///< File system storage
};

/**
 * @brief Platform types supported by the library
 */
enum class PlatformType : uint8_t {
    ARDUINO = 0,        ///< Arduino platform
    ESP32 = 1,          ///< ESP32 platform
    HOST = 2,           ///< Host/desktop platform
    CUSTOM = 3          ///< Custom platform
};

/**
 * @brief Time representation in milliseconds
 */
struct TimeMs {
    using rep = uint32_t;
    rep value;

    constexpr TimeMs() : value(0) {}
    explicit constexpr TimeMs(rep v) : value(v) {}
    
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

/**
 * @brief Wind direction in degrees (0-359)
 */
struct WindDirection {
    uint16_t degrees;
    
    constexpr WindDirection() : degrees(0) {}
    explicit constexpr WindDirection(uint16_t deg) : degrees(deg % 360) {}
    
    constexpr uint16_t getDegrees() const { return degrees; }
    constexpr double getRadians() const { return degrees * M_PI / 180.0; }
};

/**
 * @brief Wind speed in meters per second
 */
struct WindSpeed {
    float mps;
    
    constexpr WindSpeed() : mps(0.0f) {}
    explicit constexpr WindSpeed(float speed) : mps(speed) {}
    
    constexpr float getMPS() const { return mps; }
    constexpr float getKPH() const { return mps * 3.6f; }
    constexpr float getMPH() const { return mps * 2.237f; }
    constexpr float getKnots() const { return mps * 1.944f; }
};

/**
 * @brief Calibration point for wind vane
 */
struct CalibrationPoint {
    uint16_t rawValue;      ///< Raw sensor value
    WindDirection direction; ///< Corresponding wind direction
    TimeMs timestamp;       ///< Timestamp of calibration
    
    CalibrationPoint() : rawValue(0), direction(0), timestamp(0) {}
    CalibrationPoint(uint16_t raw, WindDirection dir, TimeMs time)
        : rawValue(raw), direction(dir), timestamp(time) {}
};

/**
 * @brief Calibration data structure
 */
struct CalibrationData {
    std::vector<CalibrationPoint> points;
    TimeMs lastCalibration;
    bool isValid;
    
    CalibrationData() : lastCalibration(0), isValid(false) {}
};

/**
 * @brief Wind measurement result
 */
struct WindMeasurement {
    WindDirection direction;
    WindSpeed speed;
    TimeMs timestamp;
    bool isValid;
    
    WindMeasurement() : direction(0), speed(0.0f), timestamp(0), isValid(false) {}
    WindMeasurement(WindDirection dir, WindSpeed spd, TimeMs time)
        : direction(dir), speed(spd), timestamp(time), isValid(true) {}
};

/**
 * @brief Status levels for user interface
 */
enum class StatusLevel : uint8_t {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
    DEBUG = 3
};

} // namespace WindVane