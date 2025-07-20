#ifndef WINDVANE_H
#define WINDVANE_H

#include <memory>
#include <cstdint>
#include "../Interfaces/IAnalogDigitalConverter.h"
#include "../Interfaces/ICalibrationStorage.h"
#include "../Interfaces/IDiagnostics.h"
#include "../Interfaces/IUserInterface.h"

// Forward declarations
class CalibrationManager;
struct CalibrationResult;

/**
 * @brief Main wind vane sensor class
 * 
 * This class represents a wind direction sensor that can be calibrated
 * and provides accurate directional readings. Follows standard C++ 
 * naming conventions with filename matching class name.
 */
class WindVane {
public:
    /**
     * @brief Configuration structure for WindVane dependencies
     */
    struct Config {
        IAnalogDigitalConverter& adc;
        ICalibrationStorage* storage = nullptr;
        IUserInterface& userInterface;
        IDiagnostics& diagnostics;
    };

    /**
     * @brief Construct wind vane with required dependencies
     * @param config Configuration containing all required dependencies
     */
    explicit WindVane(const Config& config);

    // Rule of Five - Make class move-only for embedded efficiency
    ~WindVane() = default;
    WindVane(const WindVane&) = delete;
    WindVane& operator=(const WindVane&) = delete;
    WindVane(WindVane&&) = default;
    WindVane& operator=(WindVane&&) = default;

    /**
     * @brief Get current wind direction in degrees
     * @return Wind direction (0-359 degrees, 0 = North)
     */
    float GetDirection() const;

    /**
     * @brief Get raw sensor reading without calibration
     * @return Raw sensor value (0.0-1.0)
     */
    float GetRawReading() const;

    /**
     * @brief Start calibration process
     * @return Calibration result with success status and details
     */
    CalibrationResult StartCalibration();

    /**
     * @brief Check if sensor is calibrated and ready
     * @return true if calibration is valid
     */
    bool IsCalibrated() const;

    /**
     * @brief Clear stored calibration data
     * @return true if clearing was successful
     */
    bool ClearCalibration();

    /**
     * @brief Get timestamp of last calibration
     * @return Milliseconds since epoch of last calibration
     */
    std::uint64_t GetLastCalibrationTimestamp() const;

    /**
     * @brief Check if the wind vane hardware is functioning
     * @return true if hardware is operational
     */
    bool IsOperational() const;

private:
    // Member variables with m_ prefix following standard C++ conventions
    IAnalogDigitalConverter& m_adc;
    ICalibrationStorage* m_storage;
    IUserInterface& m_userInterface;
    IDiagnostics& m_diagnostics;
    
    std::unique_ptr<CalibrationManager> m_calibrationManager;
    
    // Private helper methods
    float NormalizeAngle(float angle) const;
    bool ValidateReading(float reading) const;
};

#endif // WINDVANE_H