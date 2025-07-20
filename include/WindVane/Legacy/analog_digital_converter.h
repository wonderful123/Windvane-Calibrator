#ifndef WIND_VANE_INTERFACES_ANALOG_DIGITAL_CONVERTER_H_
#define WIND_VANE_INTERFACES_ANALOG_DIGITAL_CONVERTER_H_

/**
 * @file analog_digital_converter.h
 * @brief Interface for analog-to-digital converter hardware abstraction
 * 
 * This interface provides a clean abstraction for reading analog sensor values
 * from various hardware platforms (ESP32, Arduino, etc.) in compliance with
 * Google C++ Style Guide naming conventions.
 */

class AnalogDigitalConverter {
 public:
  virtual ~AnalogDigitalConverter() = default;

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
};

// Type alias for backward compatibility during migration
using IADC = AnalogDigitalConverter;

#endif  // WIND_VANE_INTERFACES_ANALOG_DIGITAL_CONVERTER_H_