#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

// Forward declarations
class WindVane;
class AnalogToDigitalConverter;
class UserInterface;
class DiagnosticsService;

/**
 * @brief Main application menu controller
 * 
 * Provides a clean, extensible menu system for the wind vane application.
 * Uses command pattern for menu actions to support easy extension.
 */
class MainMenu {
private:
    UserInterface& _ui;
    WindVane& _windVane;
    DiagnosticsService& _diagnostics;
    
    struct MenuAction {
        std::string description;
        std::function<void()> handler;
    };
    
    std::unordered_map<char, MenuAction> _actions;
    bool _running;
    
public:
    /**
     * @brief Construct main menu with required dependencies
     */
    MainMenu(UserInterface& ui, 
             WindVane& windVane, 
             DiagnosticsService& diagnostics);
    
    /**
     * @brief Run the main menu loop
     */
    void run();
    
    /**
     * @brief Stop the menu loop
     */
    void stop();
    
    /**
     * @brief Register a custom menu action
     * @param key Input key for the action
     * @param description Display text for the action
     * @param handler Function to execute when action is selected
     */
    void registerAction(char key, 
                       const std::string& description, 
                       std::function<void()> handler);

private:
    void showMenu() const;
    void handleInput();
    void setupDefaultActions();
    
    // Default menu actions
    void handleCalibration();
    void handleDirectionDisplay();
    void handleDiagnostics();
    void handleSettings();
    void handleHelp();
};

// Backward compatibility alias
using WindVaneMenu = MainMenu;