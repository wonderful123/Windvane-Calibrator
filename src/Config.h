#pragma once
#include <string>

/** Configuration parameters for the hardware setup. */
struct DeviceConfig {
  int windVanePin = 34;              ///< GPIO connected to the wind vane
  unsigned long serialBaud = 115200; ///< Baud rate for the serial console
  size_t calibrationAddress = 0;     ///< EEPROM start for calibration data
  size_t settingsAddress = 256;      ///< EEPROM start for settings data
  size_t eepromSize = 512;           ///< Size passed to EEPROM.begin
  std::string settingsFile = "settings.cfg"; ///< Path for file based settings
};

inline DeviceConfig defaultDeviceConfig() { return DeviceConfig{}; }
