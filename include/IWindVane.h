#pragma once

class IWindVane {
public:
    virtual ~IWindVane() = default;

    // Get the current wind direction in degrees.
    virtual double getWindDirection() = 0;

    // Perform calibration using the given calibration strategy.
    virtual void performCalibration() = 0;
};