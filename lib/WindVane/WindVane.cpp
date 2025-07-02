#include "WindVane.h"
#include "Calibration/StrategyFactory.h"
#include "Storage/ICalibrationStorage.h"
#include "IO/IIOHandler.h"
#include "Diagnostics/IDiagnostics.h"

WindVane::WindVane(IADC *adc, WindVaneType type,
                   CalibrationMethod method,
                   ICalibrationStorage *storage, IIOHandler *io,
                   IDiagnostics *diag,
                   const SpinningConfig &config)
    : _adc(adc), _type(type), _storage(storage) {
    auto strategy = createCalibrationStrategy(method, adc, storage, io, diag, config);
    _calibrationManager = std::make_unique<CalibrationManager>(std::move(strategy), io, diag);
}

float WindVane::direction() {
    float raw = getRawDirection();
    return _calibrationManager ? _calibrationManager->getCalibratedData(raw)
                               : raw * 360.0f;
}

float WindVane::getRawDirection() {
    return _adc ? _adc->read() : 0.0f;
}

void WindVane::runCalibration() {
    if (_calibrationManager)
        _calibrationManager->runCalibration();
}
