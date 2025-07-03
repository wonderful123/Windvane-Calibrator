#pragma once
class INumericReader {
public:
    virtual ~INumericReader() = default;
    virtual float readFloat() = 0;
    virtual int readInt() = 0;
};
