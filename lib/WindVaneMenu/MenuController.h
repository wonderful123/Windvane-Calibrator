#pragma once
#include <memory>
#include <functional>
#include <unordered_map>

// Forward declarations
class IUserIO;
class IOutput;
class WindVane;
class IDiagnostics;

// Simple menu action interface
class IMenuAction {
public:
    virtual ~IMenuAction() = default;
    virtual void execute() = 0;
    virtual std::string getDescription() const = 0;
};

// Menu controller following SRP - only handles navigation
class MenuController {
private:
    IUserIO& _io;
    IOutput& _output;
    std::unordered_map<char, std::unique_ptr<IMenuAction>> _actions;
    bool _running = false;

public:
    MenuController(IUserIO& io, IOutput& output) 
        : _io(io), _output(output) {}
    
    // Register a menu action
    void registerAction(char key, std::unique_ptr<IMenuAction> action) {
        _actions[key] = std::move(action);
    }
    
    // Show menu and handle input
    void run() {
        _running = true;
        while (_running) {
            showMenu();
            char input = _io.readInput();
            handleInput(input);
        }
    }
    
    void stop() {
        _running = false;
    }

private:
    void showMenu() const {
        _output.writeln("=== Wind Vane Menu ===");
        for (const auto& [key, action] : _actions) {
            _output.write(&key);
            _output.write(": ");
            _output.writeln(action->getDescription().c_str());
        }
        _output.writeln("Q: Quit");
        _output.write("Choose option: ");
    }
    
    void handleInput(char input) {
        if (input == 'Q' || input == 'q') {
            _running = false;
            return;
        }
        
        auto it = _actions.find(input);
        if (it != _actions.end()) {
            it->second->execute();
        } else {
            _output.writeln("Invalid option. Try again.");
        }
    }
};

// Specific menu actions
class CalibrateAction : public IMenuAction {
private:
    WindVane& _vane;
    IDiagnostics& _diag;

public:
    CalibrateAction(WindVane& vane, IDiagnostics& diag) 
        : _vane(vane), _diag(diag) {}
    
    void execute() override {
        _diag.info("Starting calibration...");
        auto result = _vane.calibrate();
        if (result.success) {
            _diag.info("Calibration completed successfully");
        } else {
            _diag.warn("Calibration failed");
        }
    }
    
    std::string getDescription() const override {
        return "Start calibration";
    }
};

class ShowDirectionAction : public IMenuAction {
private:
    WindVane& _vane;
    IOutput& _output;

public:
    ShowDirectionAction(WindVane& vane, IOutput& output) 
        : _vane(vane), _output(output) {}
    
    void execute() override {
        float direction = _vane.getDirection();
        _output.write("Current direction: ");
        _output.write(std::to_string(direction).c_str());
        _output.writeln(" degrees");
    }
    
    std::string getDescription() const override {
        return "Show current direction";
    }
};