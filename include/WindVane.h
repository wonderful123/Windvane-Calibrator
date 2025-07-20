/**
 * @file WindVane.h
 * @brief Main public API for the WindVane library
 * 
 * This library provides a complete solution for wind vane calibration,
 * measurement, and data management with support for multiple platforms.
 */

#pragma once

#include "WindVane/Types.h"
#include "WindVane/Config.h"
#include "WindVane/WindVane.h"
#include "WindVane/Builder.h"
#include "WindVane/Platform.h"

// Core components
#include "WindVane/Core/WindVane.h"

// Calibration components
#include "WindVane/Calibration/CalibrationManager.h"
#include "WindVane/Calibration/ClusterManager.h"
#include "WindVane/Calibration/ICalibrationStrategy.h"
#include "WindVane/Calibration/SpinningMethod.h"
#include "WindVane/Calibration/StrategyFactory.h"

// Storage components
#include "WindVane/Storage/ICalibrationStorage.h"
#include "WindVane/Storage/EEPROMCalibrationStorage.h"
#include "WindVane/Storage/FileCalibrationStorage.h"
#include "WindVane/Storage/IStorage.h"
#include "WindVane/Storage/SettingsManager.h"

// UI components
#include "WindVane/UI/IInputOutput.h"
#include "WindVane/UI/SerialIOHandler.h"
#include "WindVane/UI/ConsoleIOHandler.h"
#include "WindVane/UI/IOFactory.h"
#include "WindVane/UI/IIO.h"
#include "WindVane/UI/SerialOutput.h"
#include "WindVane/UI/ConsoleOutput.h"

// Menu components
#include "WindVane/Menu/WindVaneMenu.h"
#include "WindVane/Menu/MenuController.h"
#include "WindVane/Menu/SettingsMenu.h"
#include "WindVane/Menu/DiagnosticsMenu.h"

// Driver components
#include "WindVane/Drivers/ADC.h"

// Platform components
#include "WindVane/Platform/IPlatform.h"
#include "WindVane/Platform/Platform.h"
#include "WindVane/Platform/TimeUtils.h"

// Interface components
#include "WindVane/Interfaces/IDiagnostics.h"
#include "WindVane/Interfaces/IUserIO.h"
#include "WindVane/Interfaces/ICalibrationStorage.h"
#include "WindVane/Interfaces/IADC.h"
#include "WindVane/Interfaces/IAnalogDigitalConverter.h"
#include "WindVane/Interfaces/BufferedDiagnostics.h"
#include "WindVane/Interfaces/BasicDiagnostics.h"
#include "WindVane/Interfaces/ConsoleDiagnostics.h"
#include "WindVane/Interfaces/DiagnosticsBus.h"
#include "WindVane/Interfaces/DiagnosticsEvent.h"
#include "WindVane/Interfaces/IDiagnosticsSink.h"
#include "WindVane/Interfaces/SerialDiagnostics.h"

/**
 * @namespace WindVane
 * @brief Main namespace for the WindVane library
 */
namespace WindVane {

/**
 * @brief Initialize the WindVane library
 * 
 * This function should be called before using any WindVane functionality.
 * It sets up platform-specific configurations and initializes internal systems.
 * 
 * @param config Optional configuration parameters
 * @return true if initialization was successful, false otherwise
 */
bool begin(const Config& config = Config{});

/**
 * @brief Clean up the WindVane library
 * 
 * This function should be called when the library is no longer needed.
 * It performs cleanup of resources and internal systems.
 */
void end();

/**
 * @brief Get the library version string
 * 
 * @return Version string in format "major.minor.patch"
 */
const char* getVersion();

/**
 * @brief Get the library build information
 * 
 * @return Build information string
 */
const char* getBuildInfo();

} // namespace WindVane