/**
 * @file Builder.h
 * @brief Builder pattern for creating WindVane instances
 */

#pragma once

#include "WindVane.h"
#include "Config.h"
#include <memory>

namespace WindVane {

// Forward declarations
class IADC;
class ICalibrationStorage;
class IUserIO;
class IDiagnostics;

/**
 * @brief Builder class for creating WindVane instances
 * 
 * This class provides a fluent interface for configuring and creating
 * WindVane instances with proper dependency injection.
 */
class WindVaneBuilder {
public:
    /**
     * @brief Constructor
     */
    WindVaneBuilder();
    
    /**
     * @brief Destructor
     */
    ~WindVaneBuilder();
    
    // Disable copy constructor and assignment
    WindVaneBuilder(const WindVaneBuilder&) = delete;
    WindVaneBuilder& operator=(const WindVaneBuilder&) = delete;
    
    // Allow move constructor and assignment
    WindVaneBuilder(WindVaneBuilder&&) = default;
    WindVaneBuilder& operator=(WindVaneBuilder&&) = default;
    
    /**
     * @brief Set the wind vane type
     * 
     * @param type Wind vane type
     * @return Reference to this builder
     */
    WindVaneBuilder& setVaneType(VaneType type);
    
    /**
     * @brief Set the calibration method
     * 
     * @param method Calibration method
     * @return Reference to this builder
     */
    WindVaneBuilder& setCalibrationMethod(CalibrationMethod method);
    
    /**
     * @brief Set the storage type
     * 
     * @param type Storage type
     * @return Reference to this builder
     */
    WindVaneBuilder& setStorageType(StorageType type);
    
    /**
     * @brief Set the platform type
     * 
     * @param type Platform type
     * @return Reference to this builder
     */
    WindVaneBuilder& setPlatformType(PlatformType type);
    
    /**
     * @brief Set the ADC configuration
     * 
     * @param config ADC configuration
     * @return Reference to this builder
     */
    WindVaneBuilder& setADCConfig(const ADCConfig& config);
    
    /**
     * @brief Set the storage configuration
     * 
     * @param config Storage configuration
     * @return Reference to this builder
     */
    WindVaneBuilder& setStorageConfig(const StorageConfig& config);
    
    /**
     * @brief Set the wind vane configuration
     * 
     * @param config Wind vane configuration
     * @return Reference to this builder
     */
    WindVaneBuilder& setWindVaneConfig(const WindVaneConfig& config);
    
    /**
     * @brief Set the main configuration
     * 
     * @param config Main configuration
     * @return Reference to this builder
     */
    WindVaneBuilder& setConfig(const Config& config);
    
    /**
     * @brief Set the ADC interface
     * 
     * @param adc ADC interface
     * @return Reference to this builder
     */
    WindVaneBuilder& setADC(std::unique_ptr<IADC> adc);
    
    /**
     * @brief Set the storage interface
     * 
     * @param storage Storage interface
     * @return Reference to this builder
     */
    WindVaneBuilder& setStorage(std::unique_ptr<ICalibrationStorage> storage);
    
    /**
     * @brief Set the user interface
     * 
     * @param io User interface
     * @return Reference to this builder
     */
    WindVaneBuilder& setUserIO(std::unique_ptr<IUserIO> io);
    
    /**
     * @brief Set the diagnostics interface
     * 
     * @param diagnostics Diagnostics interface
     * @return Reference to this builder
     */
    WindVaneBuilder& setDiagnostics(std::unique_ptr<IDiagnostics> diagnostics);
    
    /**
     * @brief Build the WindVane instance
     * 
     * @return Unique pointer to the created WindVane instance
     * @throws std::runtime_error if required dependencies are missing
     */
    std::unique_ptr<WindVane> build();
    
    /**
     * @brief Create a default builder for Arduino platform
     * 
     * @return Configured builder for Arduino
     */
    static WindVaneBuilder arduino();
    
    /**
     * @brief Create a default builder for ESP32 platform
     * 
     * @return Configured builder for ESP32
     */
    static WindVaneBuilder esp32();
    
    /**
     * @brief Create a default builder for host platform
     * 
     * @return Configured builder for host
     */
    static WindVaneBuilder host();
    
    /**
     * @brief Create a builder with default configuration
     * 
     * @return Builder with default configuration
     */
    static WindVaneBuilder create();

private:
    Config config_;
    std::unique_ptr<IADC> adc_;
    std::unique_ptr<ICalibrationStorage> storage_;
    std::unique_ptr<IUserIO> io_;
    std::unique_ptr<IDiagnostics> diagnostics_;
    
    // Internal methods
    void createDefaultDependencies();
    void validateConfiguration() const;
    std::unique_ptr<IADC> createDefaultADC() const;
    std::unique_ptr<ICalibrationStorage> createDefaultStorage() const;
    std::unique_ptr<IUserIO> createDefaultUserIO() const;
    std::unique_ptr<IDiagnostics> createDefaultDiagnostics() const;
};

} // namespace WindVane