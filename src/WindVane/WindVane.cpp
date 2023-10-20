#include "WindVane.h"

// Constructor to initialize pin and calibration strategy
WindVane::WindVane(int pin, ICalibrationStrategy* calibrationStrategy)
    : pin_(pin), calibrationStrategy_(calibrationStrategy), calibratedDirection_(0.0) {
    // You may add initialization code here if needed
}

// Destructor
WindVane::~WindVane() {
    // Clean up resources if needed
}

// Get the current wind direction in degrees
double WindVane::getWindDirection() {
    double rawWindDirection = 0.0;  // Read raw wind direction from the pin (placeholder)
    // TODO: Implement code to read raw wind direction from hardware pin

    calibratedDirection_ = calibrationStrategy_->calibrate(rawWindDirection);
    return calibratedDirection_;
}

// Perform calibration using the given strategy
void WindVane::performCalibration() {
    double rawWindDirection = 0.0;  // Read raw wind direction from the pin (placeholder)
    // TODO: Implement code to read raw wind direction for calibration

    calibratedDirection_ = calibrationStrategy_->calibrate(rawWindDirection);
}
