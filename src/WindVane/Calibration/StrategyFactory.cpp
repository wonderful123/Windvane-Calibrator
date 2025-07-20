#include "StrategyFactory.h"
#include "SpinningMethod.h"

std::unique_ptr<ICalibrationStrategy> createCalibrationStrategy(
    const StrategyContext &ctx) {
    switch (ctx.method) {
    case CalibrationMethod::SPINNING:
    default: {
        SpinningMethodDeps deps{ctx.adc, *ctx.storage, ctx.diag, ctx.config.spin};
        return std::make_unique<SpinningMethod>(deps);
    }
    }
}
