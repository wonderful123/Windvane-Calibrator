#include "WindVane.h"
#include "Calibration/Strategies/SpinningMethod.h"
#include "Storage/ICalibrationStorage.h"
#include "IO/IIOHandler.h"
#include "Diagnostics/IDiagnostics.h"

WindVane::WindVane(IADC *adc, WindVaneType type,
                   CalibrationMethod method,
                   ICalibrationStorage *storage, IIOHandler *io,
                   IDiagnostics *diag)
    : _adc(adc), _type(type), _storage(storage) {
    _calibrationManager = new CalibrationManager(
        new SpinningMethod(adc, storage, io, diag), io, diag);
}

void WindVane::startCalibration() {
    _calibrationManager->startCalibration();
    _calibrationManager->beginCalibration();
}

void WindVane::stopCalibration() {
    _calibrationManager->endCalibration();
}
