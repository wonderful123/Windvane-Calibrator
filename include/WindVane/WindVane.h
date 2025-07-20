/**
 * @file WindVane.h
 * @brief Main WindVane class for wind direction measurement and calibration
 */

#pragma once

#include "Types.h"
#include "Config.h"
#include <memory>

namespace WindVane {

// Forward declarations
class IADC;
class ICalibrationStorage;
class IUserIO;
class IDiagnostics;

/**
 * @brief Main WindVane class for wind direction measurement and calibration
 * 
 * This class provides a complete interface for wind vane operations including:
 * - Wind direction measurement
 * - Calibration management
 * - Data storage and retrieval
 * - Platform abstraction
 */
class WindVane {
public:
    /**
     * @brief Constructor
     * 
     * @param config Configuration for the wind vane
     * @param adc ADC interface for reading sensor values
     * @param storage Storage interface for calibration data
     * @param io User interface for interaction
     * @param diagnostics Diagnostics interface for logging
     */
    WindVane(const WindVaneConfig& config,
              std::unique_ptr<IADC> adc,
              std::unique_ptr<ICalibrationStorage> storage,
              std::unique_ptr<IUserIO> io,
              std::unique_ptr<IDiagnostics> diagnostics);
    
    /**
     * @brief Destructor
     */
    ~WindVane();
    
    // Disable copy constructor and assignment
    WindVane(const WindVane&) = delete;
    WindVane& operator=(const WindVane&) = delete;
    
    // Allow move constructor and assignment
    WindVane(WindVane&&) = default;
    WindVane& operator=(WindVane&&) = default;
    
    /**
     * @brief Initialize the wind vane
     * 
     * @return true if initialization was successful
     */
    bool begin();
    
    /**
     * @brief Clean up the wind vane
     */
    void end();
    
    /**
     * @brief Get the current wind direction
     * 
     * @return Wind direction in degrees (0-359)
     */
    WindDirection getDirection() const;
    
    /**
     * @brief Get the current wind measurement
     * 
     * @return Complete wind measurement including direction, speed, and timestamp
     */
    WindMeasurement getMeasurement() const;
    
    /**
     * @brief Check if the wind vane is calibrated
     * 
     * @return true if calibration is valid
     */
    bool isCalibrated() const;
    
    /**
     * @brief Start calibration process
     * 
     * @param method Calibration method to use
     * @return true if calibration started successfully
     */
    bool startCalibration(CalibrationMethod method = CalibrationMethod::SPINNING);
    
    /**
     * @brief Stop calibration process
     * 
     * @return true if calibration was completed successfully
     */
    bool stopCalibration();
    
    /**
     * @brief Check if calibration is in progress
     * 
     * @return true if calibration is currently running
     */
    bool isCalibrating() const;
    
    /**
     * @brief Get calibration progress (0-100)
     * 
     * @return Calibration progress percentage
     */
    uint8_t getCalibrationProgress() const;
    
    /**
     * @brief Get calibration data
     * 
     * @return Current calibration data
     */
    CalibrationData getCalibrationData() const;
    
    /**
     * @brief Load calibration data from storage
     * 
     * @return true if calibration data was loaded successfully
     */
    bool loadCalibration();
    
    /**
     * @brief Save calibration data to storage
     * 
     * @return true if calibration data was saved successfully
     */
    bool saveCalibration();
    
    /**
     * @brief Clear calibration data
     * 
     * @return true if calibration data was cleared successfully
     */
    bool clearCalibration();
    
    /**
     * @brief Get raw sensor value
     * 
     * @return Raw ADC value from the sensor
     */
    uint16_t getRawValue() const;
    
    /**
     * @brief Get sensor voltage
     * 
     * @return Voltage in millivolts
     */
    uint16_t getVoltage() const;
    
    /**
     * @brief Update the wind vane (call in main loop)
     * 
     * This method should be called regularly to update measurements
     * and handle calibration processes.
     */
    void update();
    
    /**
     * @brief Get the configuration
     * 
     * @return Current configuration
     */
    const WindVaneConfig& getConfig() const { return config_; }
    
    /**
     * @brief Set a new configuration
     * 
     * @param config New configuration
     */
    void setConfig(const WindVaneConfig& config);
    
    /**
     * @brief Get library version
     * 
     * @return Version string
     */
    static const char* getVersion();
    
    /**
     * @brief Get build information
     * 
     * @return Build information string
     */
    static const char* getBuildInfo();

private:
    WindVaneConfig config_;
    std::unique_ptr<IADC> adc_;
    std::unique_ptr<ICalibrationStorage> storage_;
    std::unique_ptr<IUserIO> io_;
    std::unique_ptr<IDiagnostics> diagnostics_;
    
    CalibrationData calibrationData_;
    WindMeasurement lastMeasurement_;
    bool isCalibrating_;
    TimeMs calibrationStartTime_;
    uint8_t calibrationProgress_;
    
    // Internal methods
    void updateMeasurement();
    void updateCalibration();
    WindDirection calculateDirection(uint16_t rawValue) const;
    bool validateCalibration() const;
    void logDiagnostics(const std::string& message, StatusLevel level = StatusLevel::INFO);
};

} // namespace WindVane