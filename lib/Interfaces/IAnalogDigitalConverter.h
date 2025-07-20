#ifndef IANALOGDIGITALCONVERTER_H
#define IANALOGDIGITALCONVERTER_H

/**
 * @file IAnalogDigitalConverter.h
 * @brief Interface for analog-to-digital converter hardware abstraction
 * 
 * This interface provides a clean abstraction for reading analog sensor values
 * from various hardware platforms following standard C++ naming conventions.
 */

/**
 * @brief Interface for analog-to-digital converter hardware abstraction
 */
class IAnalogDigitalConverter {
public:
    virtual ~IAnalogDigitalConverter() = default;

    /**
     * @brief Read normalized analog value from the sensor
     * @return Normalized value between 0.0 and 1.0
     */
    virtual float ReadNormalized() const = 0;

    /**
     * @brief Read raw analog value from the sensor
     * @return Raw ADC value (platform-specific range)
     */
    virtual int ReadRaw() const = 0;

    /**
     * @brief Check if the ADC is available and functioning
     * @return true if ADC is ready for reading
     */
    virtual bool IsAvailable() const = 0;

    /**
     * @brief Get the resolution of this ADC in bits
     * @return ADC resolution (e.g., 12 for ESP32, 10 for Arduino Uno)
     */
    virtual int GetResolutionBits() const = 0;

    /**
     * @brief Get the reference voltage for this ADC
     * @return Reference voltage in volts (e.g., 3.3V for ESP32)
     */
    virtual float GetReferenceVoltage() const = 0;
};

#endif // IANALOGDIGITALCONVERTER_H