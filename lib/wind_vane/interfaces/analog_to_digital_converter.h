#pragma once

/**
 * @brief Interface for analog-to-digital converter hardware abstraction
 * 
 * This interface provides a clean abstraction for reading analog sensor values
 * from various hardware platforms (ESP32, Arduino, etc.).
 */
class AnalogToDigitalConverter {
public:
    virtual ~AnalogToDigitalConverter() = default;
    
    /**
     * @brief Read normalized analog value from the sensor
     * @return Normalized value between 0.0 and 1.0
     */
    virtual float readNormalized() const = 0;
    
    /**
     * @brief Read raw analog value from the sensor
     * @return Raw ADC value (platform-specific range)
     */
    virtual int readRaw() const = 0;
    
    /**
     * @brief Check if the ADC is available and functioning
     * @return true if ADC is ready for reading
     */
    virtual bool isAvailable() const = 0;
    
    /**
     * @brief Get the resolution of this ADC in bits
     * @return ADC resolution (e.g., 12 for ESP32, 10 for Arduino Uno)
     */
    virtual int getResolutionBits() const = 0;
};

// Type alias for backward compatibility
using IADC = AnalogToDigitalConverter;