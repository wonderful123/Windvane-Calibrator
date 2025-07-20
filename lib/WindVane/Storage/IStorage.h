#pragma once
#include <vector>
#include <cstdint>
#include <Platform/TimeUtils.h>
#include "../Calibration/ClusterData.h"
#include "StorageResult.h"

// Single, focused storage interface following LSP
template<typename T>
class IStorage {
public:
    virtual ~IStorage() = default;
    virtual StorageResult save(const T& data) = 0;
    virtual StorageResult load(T& data) = 0;
    virtual StorageResult clear() = 0;
    virtual bool isAvailable() const = 0;
};

// Specialized storage types
struct CalibrationData {
    std::vector<ClusterData> clusters;
    int version = 0;
    platform::TimeMs timestamp{0};
};

using ICalibrationStorageV2 = IStorage<CalibrationData>;

// Binary blob storage for advanced use cases
class IBlobStorage {
public:
    virtual ~IBlobStorage() = default;
    virtual StorageResult writeBlob(const std::vector<unsigned char>& data) = 0;
    virtual StorageResult readBlob(std::vector<unsigned char>& data) = 0;
};

// Adapter for backward compatibility with existing ICalibrationStorage
class CalibrationStorageAdapter : public ICalibrationStorageV2 {
private:
    ICalibrationStorage& _legacy;
    
public:
    explicit CalibrationStorageAdapter(ICalibrationStorage& legacy) 
        : _legacy(legacy) {}
        
    StorageResult save(const CalibrationData& data) override {
        return _legacy.save(data.clusters, data.version);
    }
    
    StorageResult load(CalibrationData& data) override {
        return _legacy.load(data.clusters, data.version);
    }
    
    StorageResult clear() override {
        return _legacy.clear();
    }
    
    bool isAvailable() const override {
        // Assume available if storage exists
        return true;
    }
};