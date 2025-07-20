#include "WindVane.h"
#include <algorithm>
#include <cmath>
#include "Calibration/CalibrationManager.h"
#include "Calibration/CalibrationResult.h"

// Constants for wind direction calculation
namespace {
    constexpr float kDegreesPerRevolution = 360.0f;
    constexpr float kNormalizationFactor = 1.0f;
    constexpr float kMinValidReading = 0.0f;
    constexpr float kMaxValidReading = 1.0f;
}

WindVane::WindVane(const Config& config)
    : m_adc(config.adc),
      m_storage(config.storage),
      m_userInterface(config.userInterface),
      m_diagnostics(config.diagnostics) {
    
    // Initialize calibration manager with dependencies
    CalibrationManager::Config calConfig{
        .adc = m_adc,
        .storage = m_storage,
        .userInterface = m_userInterface,
        .diagnostics = m_diagnostics
    };
    
    m_calibrationManager = std::make_unique<CalibrationManager>(calConfig);
    
    m_diagnostics.LogInfo("WindVane initialized successfully");
}

float WindVane::GetDirection() const {
    const float rawReading = GetRawReading();
    
    if (!IsCalibrated()) {
        m_diagnostics.LogWarning("Wind vane not calibrated, returning raw reading");
        return NormalizeAngle(rawReading * kDegreesPerRevolution);
    }
    
    return m_calibrationManager->ApplyCalibration(rawReading);
}

float WindVane::GetRawReading() const {
    if (!m_adc.IsAvailable()) {
        m_diagnostics.LogError("ADC not available for reading");
        return 0.0f;
    }
    
    const float reading = m_adc.ReadNormalized();
    
    if (!ValidateReading(reading)) {
        m_diagnostics.LogError("ADC reading out of range: " + std::to_string(reading));
        return std::clamp(reading, kMinValidReading, kMaxValidReading);
    }
    
    return reading;
}

CalibrationResult WindVane::StartCalibration() {
    if (!m_calibrationManager) {
        CalibrationResult result;
        result.success = false;
        result.errorMessage = "Calibration manager not initialized";
        return result;
    }
    
    m_diagnostics.LogInfo("Starting wind vane calibration");
    const CalibrationResult result = m_calibrationManager->RunCalibration();
    
    if (result.success) {
        m_diagnostics.LogInfo("Calibration completed successfully");
    } else {
        m_diagnostics.LogError("Calibration failed: " + result.errorMessage);
    }
    
    return result;
}

bool WindVane::IsCalibrated() const {
    return m_calibrationManager && 
           m_calibrationManager->HasValidCalibration();
}

bool WindVane::ClearCalibration() {
    if (!m_storage) {
        m_diagnostics.LogWarning("No storage available for clearing calibration");
        return false;
    }
    
    const bool success = m_storage->Clear();
    if (success) {
        m_diagnostics.LogInfo("Calibration data cleared");
        // Reset calibration manager state
        if (m_calibrationManager) {
            m_calibrationManager->InvalidateCalibration();
        }
    } else {
        m_diagnostics.LogError("Failed to clear calibration data");
    }
    
    return success;
}

std::uint64_t WindVane::GetLastCalibrationTimestamp() const {
    if (!m_storage) {
        return 0;
    }
    
    return m_storage->GetLastTimestamp();
}

bool WindVane::IsOperational() const {
    return m_adc.IsAvailable() && 
           (m_storage == nullptr || m_storage->IsHealthy()) &&
           ValidateReading(m_adc.ReadNormalized());
}

// Private helper methods
float WindVane::NormalizeAngle(float angle) const {
    while (angle < 0.0f) {
        angle += kDegreesPerRevolution;
    }
    while (angle >= kDegreesPerRevolution) {
        angle -= kDegreesPerRevolution;
    }
    return angle;
}

bool WindVane::ValidateReading(float reading) const {
    return reading >= kMinValidReading && reading <= kMaxValidReading;
}