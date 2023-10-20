#pragma once

class ICalibrationStrategy {
public:
    virtual ~ICalibrationStrategy() = default;

    // Calibrate the wind vane and return the corrected wind direction in degrees.
    virtual double calibrate(double rawWindDirection) = 0;
};