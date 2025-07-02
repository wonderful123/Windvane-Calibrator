#pragma once

#include "Calibration/CalibrationMethod.h"
#include "IADC.h"
#include "Calibration/CalibrationManager.h"
#include "Calibration/StrategyFactory.h"
#include "Storage/ICalibrationStorage.h"
#include "IO/IIOHandler.h"
#include "Diagnostics/IDiagnostics.h"
#include <memory>

/**
 * @enum WindVaneType
 * @brief Enumeration for different types of wind vanes.
 *
 * REED_SWITCH_WIND_VANE: Type of wind vane using reed switches.
 * MODBUS_WIND_VANE: Wind vane with Modbus interface.
 */
enum class WindVaneType {
  REED_SWITCH
  // MODBUS_WIND_VANE
  // ... other types
};

/**
 * @class WindVane
 *
 * @brief Handles wind direction sensing and calibration.
 *
 * This class encapsulates the functionality for reading wind direction and
 * calibrating the reading. It delegates the low-level hardware interaction
 * to an object that adheres to the IADC interface.
 */
class WindVane {
public:
  /**
   * @brief Constructs a WindVane object.
   *
   * @tparam HARDWARE The type of hardware interface used to read wind
   * direction.
   * @param adc The ADC interface instance.
   * @param type The type of wind vane sensor.
   * @param method The calibration method to be used.
   *
   * The constructor delegates the responsibility of hardware interaction
   * to an IADC object, which is determined by the HARDWARE parameter.
   */
  WindVane(IADC *adc, WindVaneType type, CalibrationMethod method,
           ICalibrationStorage *storage, IIOHandler *io, IDiagnostics *diag);

  /**
   * @brief Gets the calibrated wind direction.
   *
   * @return The calibrated wind direction in degrees.
   */
  float direction();

  /**
   * @brief Initiates the calibration process.
   *
   * Starts the calibration process using the selected calibration method.
   */
  void startCalibration();
  void stopCalibration();

private:
  /**
   * @brief Gets the raw wind direction from the ADC interface.
   *
   * @return The raw wind direction in degrees.
   */
  float getRawDirection();

  /// Pointer to the ADC interface.
  IADC *_adc;
  WindVaneType _type;
  std::unique_ptr<CalibrationManager> _calibrationManager;
  ICalibrationStorage *_storage;
};
