#pragma once

#include "IWindVane.h"
#include "CalibrationStrategies/ICalibrationStrategy.h"

class WindVane : public IWindVane {
public:
    WindVane(int pin, ICalibrationStrategy* calibrationStrategy);
    ~WindVane();

    double getWindDirection() override;
    void performCalibration() override;

private:
    int pin_;
    ICalibrationStrategy* calibrationStrategy_;
    double calibratedDirection_;
};