#pragma once
#include "../CalibrationMethod.h"
#include "ICalibrationStrategy.h"
#include "../CalibrationConfig.h"
#include "../../IADC.h"
#include "../../Storage/ICalibrationStorage.h"
#include "../../Diagnostics/IDiagnostics.h"
#include <memory>

struct StrategyContext {
    CalibrationMethod method{CalibrationMethod::SPINNING};
    IADC& adc;
    ICalibrationStorage* storage{nullptr};
    IDiagnostics& diag;
    CalibrationConfig config{};
};

std::unique_ptr<ICalibrationStrategy> createCalibrationStrategy(
    const StrategyContext &ctx);
