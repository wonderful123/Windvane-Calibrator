#pragma once
#include <vector>
#include <cstddef>
#include "StorageResult.h"

class IBlobStorage {
public:
    virtual ~IBlobStorage() = default;
    virtual StorageResult writeBlob(const std::vector<unsigned char>& data) = 0;
    virtual StorageResult readBlob(std::vector<unsigned char>& data) = 0;
    virtual StorageResult clear() = 0;
};
