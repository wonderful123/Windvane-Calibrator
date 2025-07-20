#include "MenuLogic.h"

#include <Platform/IPlatform.h>

WindVaneStatus MenuLogic::queryStatus(WindVane* vane, platform::TimeMs lastCalibration,
                                              IPlatform& platform) const {
    WindVaneStatus status;
    if (vane) {
        status.direction = vane->getDirection();
        status.calibrationStatus = vane->getCalibrationStatus();
    }
    status.minutesSinceCalibration = platform::toEmbedded(
        platform.millis() - lastCalibration) / 60000UL;
    return status;
}

const char* MenuLogic::statusText(CalibrationManager::CalibrationStatus st) const {
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
