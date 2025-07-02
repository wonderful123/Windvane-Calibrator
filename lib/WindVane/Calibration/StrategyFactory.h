#pragma once
#include "Calibrator.h"
#include "Strategies/ICalibrationStrategy.h"
#include "../IADC.h"
#include "../Storage/ICalibrationStorage.h"
#include "../IO/IIOHandler.h"
#include "../Diagnostics/IDiagnostics.h"
#include <memory>

std::unique_ptr<ICalibrationStrategy> createCalibrationStrategy(
    CalibrationMethod method,
    IADC *adc,
    ICalibrationStorage *storage,
    IIOHandler *io,
    IDiagnostics *diag);
