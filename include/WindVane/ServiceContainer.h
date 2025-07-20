#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <stdexcept>

// Simple dependency injection container
class ServiceContainer {
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> _services;
    std::unordered_map<std::type_index, std::function<std::shared_ptr<void>()>> _factories;

public:
    // Register a singleton instance
    template<typename TInterface, typename TImplementation>
    void registerSingleton(std::shared_ptr<TImplementation> instance) {
        static_assert(std::is_base_of_v<TInterface, TImplementation>, 
                      "TImplementation must inherit from TInterface");
        _services[std::type_index(typeid(TInterface))] = instance;
    }
    
    // Register a factory function
    template<typename TInterface>
    void registerFactory(std::function<std::shared_ptr<TInterface>()> factory) {
        _factories[std::type_index(typeid(TInterface))] = 
            [factory]() -> std::shared_ptr<void> {
                return std::static_pointer_cast<void>(factory());
            };
    }
    
    // Get a service instance
    template<typename T>
    std::shared_ptr<T> get() {
        auto typeIndex = std::type_index(typeid(T));
        
        // Check for existing singleton
        auto serviceIt = _services.find(typeIndex);
        if (serviceIt != _services.end()) {
            return std::static_pointer_cast<T>(serviceIt->second);
        }
        
        // Check for factory
        auto factoryIt = _factories.find(typeIndex);
        if (factoryIt != _factories.end()) {
            auto instance = factoryIt->second();
            return std::static_pointer_cast<T>(instance);
        }
        
        throw std::runtime_error("Service not registered: " + std::string(typeid(T).name()));
    }
    
    // Check if service is registered
    template<typename T>
    bool isRegistered() const {
        auto typeIndex = std::type_index(typeid(T));
        return _services.find(typeIndex) != _services.end() || 
               _factories.find(typeIndex) != _factories.end();
    }
    
    // Clear all registrations (useful for testing)
    void clear() {
        _services.clear();
        _factories.clear();
    }
};