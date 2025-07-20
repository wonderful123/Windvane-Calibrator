#pragma once
class IADC {
public:
    virtual ~IADC() = default;
    virtual float read() const = 0;
};
