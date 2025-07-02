#include "WindVane.h"
#include "Calibration/Strategies/SpinningMethod.h"

WindVane::WindVane(IADC *adc, WindVaneType type,
                   CalibrationMethod method)
    : _adc(adc), _type(type) {
    // For now, always use SpinningMethod. In the future, select based on method.
    _calibrationManager = new CalibrationManager(new SpinningMethod(adc));
}

void WindVane::startCalibration() {
    _calibrationManager->startCalibration();
    _calibrationManager->beginCalibration();
}

void WindVane::stopCalibration() {
    _calibrationManager->endCalibration();
}
