#include "WindVane.h"
#include "Calibration/StrategyFactory.h"
#include "Storage/ICalibrationStorage.h"
#include "IO/IIOHandler.h"
#include "Diagnostics/IDiagnostics.h"

WindVane::WindVane(IADC *adc, WindVaneType type,
                   CalibrationMethod method,
                   ICalibrationStorage *storage, IIOHandler *io,
                   IDiagnostics *diag)
    : _adc(adc), _type(type), _storage(storage) {
    auto strategy = createCalibrationStrategy(method, adc, storage, io, diag);
    _calibrationManager = std::make_unique<CalibrationManager>(std::move(strategy), io, diag);
}

void WindVane::startCalibration() {
    _calibrationManager->startCalibration();
    _calibrationManager->beginCalibration();
}

void WindVane::stopCalibration() {
    _calibrationManager->endCalibration();
}
