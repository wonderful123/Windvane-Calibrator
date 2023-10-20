#pragma once

class IPinReader {
public:
    virtual ~IPinReader() = default;

    // Read and return the value from a hardware pin.
    virtual int readPin() = 0;
};