#pragma once

#include "../../interfaces/analog_to_digital_converter.h"

/**
 * @brief ESP32-specific analog-to-digital converter implementation
 * 
 * This class provides ESP32-specific ADC functionality with proper
 * calibration and error handling.
 */
class ESP32AnalogDriver : public AnalogToDigitalConverter {
private:
    int _gpioPin;
    int _resolution;
    float _referenceVoltage;
    
public:
    /**
     * @brief Construct ESP32 ADC driver
     * @param gpioPin GPIO pin number for analog input
     * @param resolution ADC resolution in bits (default: 12)
     * @param referenceVoltage Reference voltage (default: 3.3V)
     */
    explicit ESP32AnalogDriver(int gpioPin, 
                              int resolution = 12, 
                              float referenceVoltage = 3.3f);
    
    // AnalogToDigitalConverter interface
    float readNormalized() const override;
    int readRaw() const override;
    bool isAvailable() const override;
    int getResolutionBits() const override;
    
    // ESP32-specific methods
    int getGpioPin() const { return _gpioPin; }
    float getReferenceVoltage() const { return _referenceVoltage; }
    void setReferenceVoltage(float voltage) { _referenceVoltage = voltage; }
    
    /**
     * @brief Calibrate the ADC using ESP32's internal calibration
     * @return true if calibration successful
     */
    bool calibrate();
};

// Backward compatibility alias
using ESP32ADC = ESP32AnalogDriver;