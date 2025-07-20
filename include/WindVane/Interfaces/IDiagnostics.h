/**
 * @file IDiagnostics.h
 * @brief Diagnostics interface for logging and debugging
 */

#pragma once

#include "../Types.h"
#include <string>

namespace WindVane {

/**
 * @brief Interface for diagnostics and logging
 */
class IDiagnostics {
public:
    virtual ~IDiagnostics() = default;
    
    /**
     * @brief Initialize diagnostics
     * 
     * @return true if initialization was successful
     */
    virtual bool begin() = 0;
    
    /**
     * @brief Log a message
     * 
     * @param message Message to log
     * @param level Log level
     */
    virtual void log(const std::string& message, StatusLevel level = StatusLevel::INFO) = 0;
    
    /**
     * @brief Log an info message
     * 
     * @param message Message to log
     */
    virtual void info(const std::string& message) = 0;
    
    /**
     * @brief Log a warning message
     * 
     * @param message Message to log
     */
    virtual void warning(const std::string& message) = 0;
    
    /**
     * @brief Log an error message
     * 
     * @param message Message to log
     */
    virtual void error(const std::string& message) = 0;
    
    /**
     * @brief Log a debug message
     * 
     * @param message Message to log
     */
    virtual void debug(const std::string& message) = 0;
    
    /**
     * @brief Set log level
     * 
     * @param level Minimum log level to display
     */
    virtual void setLogLevel(StatusLevel level) = 0;
    
    /**
     * @brief Get current log level
     * 
     * @return Current log level
     */
    virtual StatusLevel getLogLevel() const = 0;
    
    /**
     * @brief Check if debug output is enabled
     * 
     * @return true if debug output is enabled
     */
    virtual bool isDebugEnabled() const = 0;
    
    /**
     * @brief Enable or disable debug output
     * 
     * @param enabled true to enable debug output
     */
    virtual void setDebugEnabled(bool enabled) = 0;
    
    /**
     * @brief Get diagnostics name
     * 
     * @return Diagnostics name string
     */
    virtual const char* getName() const = 0;
    
    /**
     * @brief Check if diagnostics is initialized
     * 
     * @return true if diagnostics is initialized
     */
    virtual bool isInitialized() const = 0;
    
    /**
     * @brief Get system information
     * 
     * @return System information string
     */
    virtual std::string getSystemInfo() const = 0;
    
    /**
     * @brief Get memory usage information
     * 
     * @return Memory usage information string
     */
    virtual std::string getMemoryInfo() const = 0;
    
    /**
     * @brief Get uptime information
     * 
     * @return Uptime information string
     */
    virtual std::string getUptimeInfo() const = 0;
    
    /**
     * @brief Flush log buffer
     */
    virtual void flush() = 0;
    
    /**
     * @brief Clear log buffer
     */
    virtual void clear() = 0;
    
    /**
     * @brief Get log statistics
     * 
     * @param totalMessages Output parameter for total messages
     * @param infoMessages Output parameter for info messages
     * @param warningMessages Output parameter for warning messages
     * @param errorMessages Output parameter for error messages
     * @param debugMessages Output parameter for debug messages
     */
    virtual void getStatistics(size_t& totalMessages, 
                             size_t& infoMessages, 
                             size_t& warningMessages, 
                             size_t& errorMessages, 
                             size_t& debugMessages) const = 0;
};

} // namespace WindVane