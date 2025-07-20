/**
 * @file WindVane.cpp
 * @brief Implementation of the main WindVane class
 */

#include "WindVane.h"
#include <algorithm>
#include <cmath>

namespace WindVane {

WindVane::WindVane(const WindVaneConfig& config,
                   std::unique_ptr<IADC> adc,
                   std::unique_ptr<ICalibrationStorage> storage,
                   std::unique_ptr<IUserIO> io,
                   std::unique_ptr<IDiagnostics> diagnostics)
    : config_(config)
    , adc_(std::move(adc))
    , storage_(std::move(storage))
    , io_(std::move(io))
    , diagnostics_(std::move(diagnostics))
    , isCalibrating_(false)
    , calibrationProgress_(0)
    , calibrationStartTime_(0) {
}

WindVane::~WindVane() {
    end();
}

bool WindVane::begin() {
    if (!adc_ || !storage_ || !io_ || !diagnostics_) {
        return false;
    }
    
    // Initialize all components
    if (!adc_->begin()) {
        diagnostics_->error("Failed to initialize ADC");
        return false;
    }
    
    if (!storage_->begin()) {
        diagnostics_->error("Failed to initialize storage");
        return false;
    }
    
    if (!io_->begin()) {
        diagnostics_->error("Failed to initialize user interface");
        return false;
    }
    
    if (!diagnostics_->begin()) {
        // Diagnostics failure is not critical
        io_->printStatus("Warning: Diagnostics initialization failed", StatusLevel::WARNING);
    }
    
    diagnostics_->info("WindVane initialized successfully");
    return true;
}

void WindVane::end() {
    if (isCalibrating_) {
        stopCalibration();
    }
    
    diagnostics_->info("WindVane shutdown complete");
}

WindDirection WindVane::getDirection() const {
    if (!isCalibrated()) {
        return WindDirection(0);
    }
    
    uint16_t rawValue = adc_->read();
    return calculateDirection(rawValue);
}

WindMeasurement WindVane::getMeasurement() const {
    if (!isCalibrated()) {
        return WindMeasurement();
    }
    
    uint16_t rawValue = adc_->read();
    WindDirection direction = calculateDirection(rawValue);
    WindSpeed speed = WindSpeed(0.0f); // Placeholder for wind speed
    
    return WindMeasurement(direction, speed, TimeMs::now());
}

bool WindVane::isCalibrated() const {
    return calibrationData_.isValid && !calibrationData_.points.empty();
}

bool WindVane::startCalibration(CalibrationMethod method) {
    if (isCalibrating_) {
        diagnostics_->warning("Calibration already in progress");
        return false;
    }
    
    // Clear existing calibration data
    calibrationData_.points.clear();
    calibrationData_.isValid = false;
    
    // Start calibration
    isCalibrating_ = true;
    calibrationProgress_ = 0;
    calibrationStartTime_ = TimeMs::now();
    
    diagnostics_->info("Calibration started with method: " + std::to_string(static_cast<int>(method)));
    io_->printStatus("Calibration started", StatusLevel::INFO);
    
    return true;
}

bool WindVane::stopCalibration() {
    if (!isCalibrating_) {
        diagnostics_->warning("No calibration in progress");
        return false;
    }
    
    // Validate calibration data
    if (validateCalibration()) {
        calibrationData_.isValid = true;
        calibrationData_.lastCalibration = TimeMs::now();
        
        diagnostics_->info("Calibration completed successfully");
        io_->printStatus("Calibration completed successfully", StatusLevel::INFO);
    } else {
        diagnostics_->error("Calibration validation failed");
        io_->printStatus("Calibration failed", StatusLevel::ERROR);
    }
    
    isCalibrating_ = false;
    calibrationProgress_ = 0;
    
    return calibrationData_.isValid;
}

bool WindVane::isCalibrating() const {
    return isCalibrating_;
}

uint8_t WindVane::getCalibrationProgress() const {
    return calibrationProgress_;
}

CalibrationData WindVane::getCalibrationData() const {
    return calibrationData_;
}

bool WindVane::loadCalibration() {
    if (!storage_) {
        diagnostics_->error("Storage not available");
        return false;
    }
    
    if (storage_->load(calibrationData_)) {
        diagnostics_->info("Calibration data loaded successfully");
        return true;
    } else {
        diagnostics_->warning("Failed to load calibration data");
        return false;
    }
}

bool WindVane::saveCalibration() {
    if (!storage_) {
        diagnostics_->error("Storage not available");
        return false;
    }
    
    if (storage_->save(calibrationData_)) {
        diagnostics_->info("Calibration data saved successfully");
        return true;
    } else {
        diagnostics_->error("Failed to save calibration data");
        return false;
    }
}

bool WindVane::clearCalibration() {
    calibrationData_.points.clear();
    calibrationData_.isValid = false;
    calibrationData_.lastCalibration = TimeMs(0);
    
    if (storage_) {
        storage_->clear();
    }
    
    diagnostics_->info("Calibration data cleared");
    return true;
}

uint16_t WindVane::getRawValue() const {
    return adc_ ? adc_->read() : 0;
}

uint16_t WindVane::getVoltage() const {
    return adc_ ? adc_->readVoltage() : 0;
}

void WindVane::update() {
    if (isCalibrating_) {
        updateCalibration();
    }
    
    updateMeasurement();
}

void WindVane::setConfig(const WindVaneConfig& config) {
    config_ = config;
    diagnostics_->info("Configuration updated");
}

const char* WindVane::getVersion() {
    return "1.0.0";
}

const char* WindVane::getBuildInfo() {
    return "WindVane Library v1.0.0 - Self-contained wind vane calibration library";
}

// Private methods

void WindVane::updateMeasurement() {
    // Update the last measurement
    lastMeasurement_ = getMeasurement();
}

void WindVane::updateCalibration() {
    if (!isCalibrating_) {
        return;
    }
    
    // Check for timeout
    TimeMs elapsed = TimeMs::now() - calibrationStartTime_;
    if (elapsed.count() > config_.timeout) {
        diagnostics_->warning("Calibration timeout");
        stopCalibration();
        return;
    }
    
    // Read current sensor value
    uint16_t rawValue = adc_->read();
    
    // Add calibration point (simplified - in real implementation, this would be more sophisticated)
    CalibrationPoint point(rawValue, WindDirection(calibrationProgress_ * 360 / 100), TimeMs::now());
    calibrationData_.points.push_back(point);
    
    // Update progress
    calibrationProgress_ = std::min(100u, static_cast<uint8_t>(calibrationData_.points.size() * 100 / config_.maxPoints));
    
    // Check if calibration is complete
    if (calibrationData_.points.size() >= config_.minPoints) {
        stopCalibration();
    }
}

WindDirection WindVane::calculateDirection(uint16_t rawValue) const {
    if (calibrationData_.points.empty()) {
        return WindDirection(0);
    }
    
    // Simple linear interpolation (in real implementation, this would be more sophisticated)
    float normalizedValue = static_cast<float>(rawValue) / adc_->getMaxValue();
    uint16_t degrees = static_cast<uint16_t>(normalizedValue * 360.0f);
    
    return WindDirection(degrees);
}

bool WindVane::validateCalibration() const {
    if (calibrationData_.points.size() < config_.minPoints) {
        return false;
    }
    
    // Check for minimum coverage (simplified validation)
    return true;
}

void WindVane::logDiagnostics(const std::string& message, StatusLevel level) {
    if (diagnostics_) {
        diagnostics_->log(message, level);
    }
}

} // namespace WindVane