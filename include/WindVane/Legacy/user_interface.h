#pragma once

#include <string>

/**
 * @brief User input interface for character-based input
 */
class CharacterInput {
public:
    virtual ~CharacterInput() = default;
    virtual bool hasInput() const = 0;
    virtual char readCharacter() const = 0;
    virtual void flushInput() const = 0;
};

/**
 * @brief User input interface for numeric values
 */
class NumericInput {
public:
    virtual ~NumericInput() = default;
    virtual float readFloat() const = 0;
    virtual int readInteger() const = 0;
};

/**
 * @brief User interaction interface for prompts and confirmations
 */
class UserInteraction {
public:
    virtual ~UserInteraction() = default;
    virtual bool confirmYesNo(const std::string& prompt) const = 0;
    virtual std::string readString(const std::string& prompt) const = 0;
};

/**
 * @brief Output interface for displaying information to user
 */
class UserOutput {
public:
    virtual ~UserOutput() = default;
    virtual void write(const std::string& text) const = 0;
    virtual void writeLine(const std::string& text) const = 0;
    virtual void clear() const = 0;
    virtual void setStatusMessage(const std::string& message) const = 0;
};

/**
 * @brief Complete user interface combining all interaction capabilities
 * 
 * This interface provides a complete abstraction for user interaction,
 * including input, output, and user prompts.
 */
class UserInterface : public CharacterInput, 
                     public NumericInput, 
                     public UserInteraction, 
                     public UserOutput {
public:
    virtual ~UserInterface() = default;
    
    /**
     * @brief Wait for specified duration
     * @param milliseconds Duration to wait in milliseconds
     */
    virtual void wait(int milliseconds) const = 0;
    
    /**
     * @brief Check if the interface is ready for interaction
     * @return true if interface is available
     */
    virtual bool isReady() const = 0;
};

// Backward compatibility aliases
using IUserIO = UserInterface;
using IOutput = UserOutput;