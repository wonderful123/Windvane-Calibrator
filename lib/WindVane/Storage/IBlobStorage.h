#pragma once
#include <vector>
#include <cstddef>

class IBlobStorage {
public:
    virtual ~IBlobStorage() = default;
    virtual bool writeBlob(const std::vector<unsigned char>& data) = 0;
    virtual bool readBlob(std::vector<unsigned char>& data) = 0;
    virtual void clear() = 0;
};
