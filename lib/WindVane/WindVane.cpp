#include "WindVane.h"
#include "Calibration/Strategies/SpinningMethod.h"

WindVane::WindVane(IADC *adc, WindVaneType type,
                   CalibrationMethod method)
    : _adc(adc), _type(type) {
    // For now, always use SpinningMethod. In the future, select based on method.
    _calibrationManager = new CalibrationManager(new SpinningMethod());
}

void WindVane::startCalibration() {
    _calibrationManager->startCalibration();
}

void WindVane::stopCalibration() {
    _calibrationManager->endCalibration();
}
