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