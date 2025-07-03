#include "WindVaneMenuLogic.h"

#include "../Platform/Platform.h"

WindVaneStatus WindVaneMenuLogic::queryStatus(WindVane* vane, unsigned long lastCalibration) const {
    WindVaneStatus status;
    if (vane) {
        status.direction = vane->direction();
        status.calibrationStatus = vane->calibrationStatus();
    }
    status.minutesSinceCalibration = (platformMillis() - lastCalibration) / 60000UL;
    return status;
}

const char* WindVaneMenuLogic::statusText(CalibrationManager::CalibrationStatus st) const {
    switch (st) {
        case CalibrationManager::CalibrationStatus::NotStarted:
            return "Uncal";
        case CalibrationManager::CalibrationStatus::AwaitingStart:
            return "Awaiting";
        case CalibrationManager::CalibrationStatus::InProgress:
            return "Calibrating";
        case CalibrationManager::CalibrationStatus::Completed:
            return "OK";
    }
    return "Unknown";
}
