#pragma once
#include "CalibrationMethod.h"
#include "Strategies/ICalibrationStrategy.h"
#include "Strategies/SpinningMethod.h"
#include "../IADC.h"
#include "../Storage/ICalibrationStorage.h"
#include "../IO/IIOHandler.h"
#include "../Diagnostics/IDiagnostics.h"
#include <memory>

struct StrategyContext {
    CalibrationMethod method{CalibrationMethod::SPINNING};
    IADC *adc{};
    ICalibrationStorage *storage{};
    IIOHandler *io{};
    IDiagnostics *diag{};
    SpinningConfig config{};
};

std::unique_ptr<ICalibrationStrategy> createCalibrationStrategy(
    const StrategyContext &ctx);
