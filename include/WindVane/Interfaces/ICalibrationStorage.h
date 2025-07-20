/**
 * @file ICalibrationStorage.h
 * @brief Calibration storage interface
 */

#pragma once

#include "../Types.h"
#include <vector>

namespace WindVane {

/**
 * @brief Interface for storing and retrieving calibration data
 */
class ICalibrationStorage {
public:
    virtual ~ICalibrationStorage() = default;
    
    /**
     * @brief Initialize the storage
     * 
     * @return true if initialization was successful
     */
    virtual bool begin() = 0;
    
    /**
     * @brief Save calibration data
     * 
     * @param data Calibration data to save
     * @return true if save was successful
     */
    virtual bool save(const CalibrationData& data) = 0;
    
    /**
     * @brief Load calibration data
     * 
     * @param data Output parameter for loaded calibration data
     * @return true if load was successful
     */
    virtual bool load(CalibrationData& data) = 0;
    
    /**
     * @brief Check if calibration data exists
     * 
     * @return true if calibration data exists
     */
    virtual bool exists() = 0;
    
    /**
     * @brief Clear calibration data
     * 
     * @return true if clear was successful
     */
    virtual bool clear() = 0;
    
    /**
     * @brief Get storage size in bytes
     * 
     * @return Storage size in bytes
     */
    virtual size_t getSize() const = 0;
    
    /**
     * @brief Get available storage space in bytes
     * 
     * @return Available storage space in bytes
     */
    virtual size_t getAvailableSpace() const = 0;
    
    /**
     * @brief Check if storage is initialized
     * 
     * @return true if storage is initialized
     */
    virtual bool isInitialized() const = 0;
    
    /**
     * @brief Get storage type
     * 
     * @return Storage type
     */
    virtual StorageType getStorageType() const = 0;
    
    /**
     * @brief Get storage name/identifier
     * 
     * @return Storage name string
     */
    virtual const char* getStorageName() const = 0;
    
    /**
     * @brief Validate stored data integrity
     * 
     * @return true if data is valid
     */
    virtual bool validate() = 0;
    
    /**
     * @brief Get storage statistics
     * 
     * @param totalBytes Output parameter for total bytes
     * @param usedBytes Output parameter for used bytes
     * @param freeBytes Output parameter for free bytes
     */
    virtual void getStatistics(size_t& totalBytes, size_t& usedBytes, size_t& freeBytes) const = 0;
    
    /**
     * @brief Backup calibration data
     * 
     * @param backupPath Path for backup file
     * @return true if backup was successful
     */
    virtual bool backup(const std::string& backupPath) = 0;
    
    /**
     * @brief Restore calibration data from backup
     * 
     * @param backupPath Path to backup file
     * @return true if restore was successful
     */
    virtual bool restore(const std::string& backupPath) = 0;
};

} // namespace WindVane