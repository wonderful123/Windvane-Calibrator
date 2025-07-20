/**
 * @file IADC.h
 * @brief ADC (Analog-to-Digital Converter) interface
 */

#pragma once

#include "../Types.h"
#include <cstdint>

namespace WindVane {

/**
 * @brief ADC interface for reading analog sensor values
 */
class IADC {
public:
    virtual ~IADC() = default;
    
    /**
     * @brief Initialize the ADC
     * 
     * @return true if initialization was successful
     */
    virtual bool begin() = 0;
    
    /**
     * @brief Read raw ADC value
     * 
     * @return Raw ADC value (0-4095 for 12-bit, 0-1023 for 10-bit, etc.)
     */
    virtual uint16_t read() = 0;
    
    /**
     * @brief Read averaged ADC value
     * 
     * @param samples Number of samples to average
     * @return Averaged ADC value
     */
    virtual uint16_t readAveraged(uint8_t samples = 1) = 0;
    
    /**
     * @brief Read voltage in millivolts
     * 
     * @return Voltage in millivolts
     */
    virtual uint16_t readVoltage() = 0;
    
    /**
     * @brief Get ADC resolution in bits
     * 
     * @return ADC resolution (8, 10, 12, etc.)
     */
    virtual uint8_t getResolution() const = 0;
    
    /**
     * @brief Get reference voltage in millivolts
     * 
     * @return Reference voltage in millivolts
     */
    virtual uint16_t getReferenceVoltage() const = 0;
    
    /**
     * @brief Get maximum ADC value
     * 
     * @return Maximum ADC value based on resolution
     */
    virtual uint16_t getMaxValue() const = 0;
    
    /**
     * @brief Set ADC pin
     * 
     * @param pin ADC pin number
     */
    virtual void setPin(uint8_t pin) = 0;
    
    /**
     * @brief Get current ADC pin
     * 
     * @return ADC pin number
     */
    virtual uint8_t getPin() const = 0;
    
    /**
     * @brief Set ADC resolution
     * 
     * @param resolution ADC resolution in bits
     */
    virtual void setResolution(uint8_t resolution) = 0;
    
    /**
     * @brief Set reference voltage
     * 
     * @param voltage Reference voltage in millivolts
     */
    virtual void setReferenceVoltage(uint16_t voltage) = 0;
    
    /**
     * @brief Check if ADC is initialized
     * 
     * @return true if ADC is initialized
     */
    virtual bool isInitialized() const = 0;
    
    /**
     * @brief Get ADC reading as percentage
     * 
     * @return ADC reading as percentage (0-100)
     */
    virtual float readPercentage() = 0;
    
    /**
     * @brief Get ADC reading as normalized value (0.0-1.0)
     * 
     * @return Normalized ADC value
     */
    virtual float readNormalized() = 0;
};

} // namespace WindVane