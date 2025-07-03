#include "StrategyFactory.h"
#include "Strategies/SpinningMethod.h"

std::unique_ptr<ICalibrationStrategy> createCalibrationStrategy(
    const StrategyContext &ctx) {
    switch (ctx.method) {
    case CalibrationMethod::SPINNING:
    default: {
        SpinningMethodDeps deps{ctx.adc, ctx.storage, ctx.io, ctx.diag, ctx.config};
        return std::make_unique<SpinningMethod>(deps);
    }
    }
}
