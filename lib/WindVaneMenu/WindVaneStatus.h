#pragma once
#include <WindVane.h>

struct WindVaneStatus {
    float direction{0.0f};
    CalibrationManager::CalibrationStatus calibrationStatus{
        CalibrationManager::CalibrationStatus::NotStarted};
    unsigned long minutesSinceCalibration{0};
};
