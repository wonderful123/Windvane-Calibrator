#include "WindVaneMenuLogic.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <chrono>
static unsigned long millis() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - start).count();
}
#endif

WindVaneStatus WindVaneMenuLogic::queryStatus(WindVane* vane, unsigned long lastCalibration) const {
    WindVaneStatus status;
    if (vane) {
        status.direction = vane->direction();
        status.calibrationStatus = vane->calibrationStatus();
    }
    status.minutesSinceCalibration = (millis() - lastCalibration) / 60000UL;
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
