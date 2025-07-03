#pragma once
#include "../SpinningConfig.h"

class ISpinningConfigurable {
public:
    virtual ~ISpinningConfigurable() = default;
    virtual void setConfig(const SpinningConfig& cfg) = 0;
    virtual SpinningConfig config() const = 0;
};
