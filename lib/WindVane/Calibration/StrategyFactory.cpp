#include "StrategyFactory.h"
#include "Strategies/SpinningMethod.h"

std::unique_ptr<ICalibrationStrategy> createCalibrationStrategy(
    CalibrationMethod method,
    IADC *adc,
    ICalibrationStorage *storage,
    IIOHandler *io,
    IDiagnostics *diag) {
    switch (method) {
    case CalibrationMethod::SPINNING:
    default:
        return std::make_unique<SpinningMethod>(adc, storage, io, diag);
    }
}
