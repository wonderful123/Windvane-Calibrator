#pragma once
class IADC {
public:
    virtual ~IADC() = default;
    virtual float read() = 0;
};
