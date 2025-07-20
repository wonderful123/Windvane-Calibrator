#pragma once
#include <DI/ServiceContainer.h>
#include <UI/IInputOutput.h>
#include <WindVane.h>
#include <WindVaneMenu/MenuController.h>
#include <memory>

class SimplifiedApp {
private:
    ServiceContainer _container;
    std::unique_ptr<MenuController> _menuController;
    
public:
    SimplifiedApp() = default;
    
    // Configure all dependencies
    void configure() {
        // This would be configured based on platform (Arduino vs Host)
        // Register platform-specific implementations
        setupPlatformServices();
        setupApplicationServices();
        setupMenuActions();
    }
    
    void run() {
        if (!_menuController) {
            throw std::runtime_error("Application not configured");
        }
        _menuController->run();
    }
    
private:
    void setupPlatformServices() {
        // Platform-specific service registration would go here
        // This demonstrates the pattern - actual implementations would 
        // be in platform-specific configuration files
        
        #ifdef ARDUINO
            // Register ESP32-specific services
            // _container.registerSingleton<IADC>(std::make_shared<ESP32ADC>(analogPin));
        #else
            // Register host/test services
            // _container.registerSingleton<IADC>(std::make_shared<NullADC>());
        #endif
    }
    
    void setupApplicationServices() {
        // Register application-level services
        // auto windVane = WindVaneBuilder()
        //     .withADC(*_container.get<IADC>())
        //     .withIO(*_container.get<IUserIO>())
        //     .withDiagnostics(*_container.get<IDiagnostics>())
        //     .withStorage(*_container.get<ICalibrationStorage>())
        //     .build();
        //     
        // _container.registerSingleton<WindVane>(windVane);
    }
    
    void setupMenuActions() {
        auto io = _container.get<IUserIO>();
        auto output = _container.get<IOutput>();
        _menuController = std::make_unique<MenuController>(*io, *output);
        
        // Register menu actions
        auto windVane = _container.get<WindVane>();
        auto diagnostics = _container.get<IDiagnostics>();
        
        _menuController->registerAction('C', 
            std::make_unique<CalibrateAction>(*windVane, *diagnostics));
        _menuController->registerAction('D', 
            std::make_unique<ShowDirectionAction>(*windVane, *output));
    }
};