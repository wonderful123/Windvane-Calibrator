/**
 * @file IUserIO.h
 * @brief User interface for input/output operations
 */

#pragma once

#include "../Types.h"
#include <string>

namespace WindVane {

/**
 * @brief Interface for user input/output operations
 */
class IUserIO {
public:
    virtual ~IUserIO() = default;
    
    /**
     * @brief Initialize the user interface
     * 
     * @return true if initialization was successful
     */
    virtual bool begin() = 0;
    
    /**
     * @brief Print a message
     * 
     * @param message Message to print
     */
    virtual void print(const std::string& message) = 0;
    
    /**
     * @brief Print a message with newline
     * 
     * @param message Message to print
     */
    virtual void println(const std::string& message) = 0;
    
    /**
     * @brief Print a message with status level
     * 
     * @param message Message to print
     * @param level Status level
     */
    virtual void printStatus(const std::string& message, StatusLevel level = StatusLevel::INFO) = 0;
    
    /**
     * @brief Read a line of input
     * 
     * @return Input string
     */
    virtual std::string readLine() = 0;
    
    /**
     * @brief Read a character
     * 
     * @return Character read
     */
    virtual char readChar() = 0;
    
    /**
     * @brief Check if input is available
     * 
     * @return true if input is available
     */
    virtual bool available() = 0;
    
    /**
     * @brief Clear the output
     */
    virtual void clear() = 0;
    
    /**
     * @brief Set text color (if supported)
     * 
     * @param color Color code
     */
    virtual void setColor(uint8_t color) = 0;
    
    /**
     * @brief Reset text color
     */
    virtual void resetColor() = 0;
    
    /**
     * @brief Check if color output is supported
     * 
     * @return true if color output is supported
     */
    virtual bool supportsColor() const = 0;
    
    /**
     * @brief Get interface name
     * 
     * @return Interface name string
     */
    virtual const char* getName() const = 0;
    
    /**
     * @brief Check if interface is initialized
     * 
     * @return true if interface is initialized
     */
    virtual bool isInitialized() const = 0;
    
    /**
     * @brief Flush output buffer
     */
    virtual void flush() = 0;
    
    /**
     * @brief Set output format
     * 
     * @param format Format string
     */
    virtual void setFormat(const std::string& format) = 0;
    
    /**
     * @brief Get current format
     * 
     * @return Current format string
     */
    virtual std::string getFormat() const = 0;
};

} // namespace WindVane