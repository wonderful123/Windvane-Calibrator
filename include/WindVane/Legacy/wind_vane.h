#ifndef WIND_VANE_CORE_WIND_VANE_H_
#define WIND_VANE_CORE_WIND_VANE_H_

#include <memory>

#include "wind_vane/interfaces/analog_digital_converter.h"
#include "wind_vane/interfaces/calibration_storage.h"
#include "wind_vane/interfaces/diagnostics.h"
#include "wind_vane/interfaces/user_input_output.h"

// Forward declarations
class CalibrationManager;
struct CalibrationResult;

/**
 * @brief Main wind vane sensor class
 * 
 * This class represents a wind direction sensor that can be calibrated
 * and provides accurate directional readings. Follows Google C++ Style
 * Guide conventions for embedded systems.
 */
class WindVane {
 public:
  /**
   * @brief Configuration structure for WindVane dependencies
   */
  struct Config {
    AnalogDigitalConverter& adc;
    CalibrationStorage* storage = nullptr;
    UserInputOutput& user_io;
    Diagnostics& diagnostics;
  };

  /**
   * @brief Construct wind vane with required dependencies
   * @param config Configuration containing all required dependencies
   */
  explicit WindVane(const Config& config);

  // Disable copy and assignment (sensor is unique)
  WindVane(const WindVane&) = delete;
  WindVane& operator=(const WindVane&) = delete;

  // Allow move construction and assignment
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
  uint64_t GetLastCalibrationTimestamp() const;

 private:
  AnalogDigitalConverter& adc_;
  CalibrationStorage* storage_;
  UserInputOutput& user_io_;
  Diagnostics& diagnostics_;
  
  std::unique_ptr<CalibrationManager> calibration_manager_;
};

// Backward compatibility alias
using WindVane_Legacy = WindVane;

#endif  // WIND_VANE_CORE_WIND_VANE_H_