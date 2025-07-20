/**
 * @file Config.h
 * @brief Configuration structures for the WindVane library
 */

#pragma once

#include "Types.h"
#include <cstdint>

namespace WindVane {

/**
 * @brief Main configuration structure for the WindVane library
 */
struct Config {
    VaneType vaneType = VaneType::REED_SWITCH;
    CalibrationMethod calibrationMethod = CalibrationMethod::SPINNING;
    StorageType storageType = StorageType::EEPROM;
    PlatformType platformType = PlatformType::ARDUINO;
    
    // ADC configuration
    uint8_t adcPin = 0;
    uint8_t adcResolution = 12;
    uint16_t adcReferenceVoltage = 3300; // mV
    
    // Calibration configuration
    uint16_t minCalibrationPoints = 8;
    uint16_t maxCalibrationPoints = 360;
    uint32_t calibrationTimeout = 300000; // 5 minutes in ms
    
    // Storage configuration
    uint16_t storageAddress = 0;
    uint16_t storageSize = 1024;
    std::string storagePath = "";
    
    // UI configuration
    bool enableSerialOutput = true;
    uint32_t serialBaudRate = 115200;
    bool enableColorOutput = false;
    
    // Platform configuration
    bool enableDiagnostics = true;
    bool enableDebugOutput = false;
    
    Config() = default;
    
    /**
     * @brief Create a default configuration for Arduino platform
     */
    static Config arduino() {
        Config config;
        config.platformType = PlatformType::ARDUINO;
        config.storageType = StorageType::EEPROM;
        config.enableColorOutput = false;
        return config;
    }
    
    /**
     * @brief Create a default configuration for ESP32 platform
     */
    static Config esp32() {
        Config config;
        config.platformType = PlatformType::ESP32;
        config.storageType = StorageType::FLASH;
        config.adcResolution = 12;
        config.enableColorOutput = false;
        return config;
    }
    
    /**
     * @brief Create a default configuration for host platform
     */
    static Config host() {
        Config config;
        config.platformType = PlatformType::HOST;
        config.storageType = StorageType::FILE_SYSTEM;
        config.enableColorOutput = true;
        config.enableDebugOutput = true;
        return config;
    }
};

/**
 * @brief Wind vane specific configuration
 */
struct WindVaneConfig {
    VaneType type;
    CalibrationMethod calibrationMethod;
    uint16_t minPoints;
    uint16_t maxPoints;
    uint32_t timeout;
    
    WindVaneConfig() 
        : type(VaneType::REED_SWITCH)
        , calibrationMethod(CalibrationMethod::SPINNING)
        , minPoints(8)
        , maxPoints(360)
        , timeout(300000) {}
        
    WindVaneConfig(VaneType t, CalibrationMethod cm, uint16_t min, uint16_t max, uint32_t to)
        : type(t)
        , calibrationMethod(cm)
        , minPoints(min)
        , maxPoints(max)
        , timeout(to) {}
};

/**
 * @brief ADC configuration
 */
struct ADCConfig {
    uint8_t pin;
    uint8_t resolution;
    uint16_t referenceVoltage;
    uint16_t sampleCount;
    uint32_t sampleInterval;
    
    ADCConfig() 
        : pin(0)
        , resolution(12)
        , referenceVoltage(3300)
        , sampleCount(1)
        , sampleInterval(100) {}
        
    ADCConfig(uint8_t p, uint8_t res, uint16_t ref, uint16_t samples, uint32_t interval)
        : pin(p)
        , resolution(res)
        , referenceVoltage(ref)
        , sampleCount(samples)
        , sampleInterval(interval) {}
};

/**
 * @brief Storage configuration
 */
struct StorageConfig {
    StorageType type;
    uint16_t address;
    uint16_t size;
    std::string path;
    
    StorageConfig() 
        : type(StorageType::EEPROM)
        , address(0)
        , size(1024) {}
        
    StorageConfig(StorageType t, uint16_t addr, uint16_t sz, const std::string& p = "")
        : type(t)
        , address(addr)
        , size(sz)
        , path(p) {}
};

} // namespace WindVane