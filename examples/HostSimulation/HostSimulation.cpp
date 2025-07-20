/**
 * @file HostSimulation.cpp
 * @brief Host simulation example for the WindVane library
 * 
 * This example demonstrates using the WindVane library on a desktop
 * platform with simulated sensor data and file-based storage.
 */

#include <WindVane.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace WindVane;

// Simulated wind vane data
class SimulatedWindVane {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> angle_dist;
    std::uniform_real_distribution<float> speed_dist;
    float current_angle;
    float current_speed;
    
public:
    SimulatedWindVane() 
        : gen(rd())
        , angle_dist(0.0f, 360.0f)
        , speed_dist(0.0f, 20.0f)
        , current_angle(0.0f)
        , current_speed(5.0f) {}
    
    void update() {
        // Simulate gradual wind changes
        current_angle += (angle_dist(gen) - 180.0f) * 0.01f;
        if (current_angle < 0) current_angle += 360.0f;
        if (current_angle >= 360) current_angle -= 360.0f;
        
        current_speed += (speed_dist(gen) - current_speed) * 0.02f;
        if (current_speed < 0) current_speed = 0;
        if (current_speed > 30) current_speed = 30;
    }
    
    uint16_t getRawValue() const {
        // Convert angle to raw ADC value (0-4095)
        return static_cast<uint16_t>((current_angle / 360.0f) * 4095.0f);
    }
    
    float getAngle() const { return current_angle; }
    float getSpeed() const { return current_speed; }
};

int main() {
    std::cout << "WindVane Host Simulation Example" << std::endl;
    std::cout << "=================================" << std::endl;
    
    // Create simulated wind vane
    SimulatedWindVane simulatedVane;
    
    // Configure WindVane for host platform
    WindVaneBuilder builder = WindVaneBuilder::host();
    builder.setVaneType(VaneType::POTENTIOMETER)
           .setCalibrationMethod(CalibrationMethod::AUTOMATIC)
           .setStorageConfig(StorageConfig(StorageType::FILE_SYSTEM, 0, 1024, "./calibration.dat"));
    
    // Build WindVane instance
    auto windVane = builder.build();
    
    if (!windVane) {
        std::cerr << "Failed to create WindVane instance!" << std::endl;
        return 1;
    }
    
    // Initialize WindVane
    if (!windVane->begin()) {
        std::cerr << "Failed to initialize WindVane!" << std::endl;
        return 1;
    }
    
    std::cout << "WindVane initialized successfully" << std::endl;
    
    // Load existing calibration if available
    if (windVane->loadCalibration()) {
        std::cout << "Loaded existing calibration" << std::endl;
    } else {
        std::cout << "No calibration found, starting automatic calibration..." << std::endl;
        
        // Start automatic calibration
        if (windVane->startCalibration(CalibrationMethod::AUTOMATIC)) {
            std::cout << "Automatic calibration started. Simulating wind vane rotation..." << std::endl;
            
            // Simulate calibration process
            for (int i = 0; i < 100; ++i) {
                simulatedVane.update();
                
                // Simulate rotation through all angles
                float angle = (i / 100.0f) * 360.0f;
                simulatedVane.update();
                
                windVane->update();
                
                if (!windVane->isCalibrating()) {
                    break;
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            
            if (windVane->stopCalibration()) {
                std::cout << "Calibration completed successfully!" << std::endl;
                windVane->saveCalibration();
            } else {
                std::cout << "Calibration failed!" << std::endl;
            }
        }
    }
    
    // Main simulation loop
    std::cout << "\nStarting wind simulation..." << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    
    for (int i = 0; i < 100; ++i) {
        // Update simulated wind
        simulatedVane.update();
        
        // Update WindVane
        windVane->update();
        
        // Get measurements
        WindDirection direction = windVane->getDirection();
        WindSpeed speed = WindSpeed(simulatedVane.getSpeed());
        uint16_t rawValue = windVane->getRawValue();
        uint16_t voltage = windVane->getVoltage();
        
        // Print measurements
        std::cout << "Simulation Step " << i + 1 << ":" << std::endl;
        std::cout << "  Simulated Angle: " << simulatedVane.getAngle() << "°" << std::endl;
        std::cout << "  Measured Direction: " << direction.getDegrees() << "°" << std::endl;
        std::cout << "  Simulated Speed: " << simulatedVane.getSpeed() << " m/s" << std::endl;
        std::cout << "  Raw ADC Value: " << rawValue << std::endl;
        std::cout << "  Voltage: " << voltage << " mV" << std::endl;
        std::cout << "  Calibrated: " << (windVane->isCalibrated() ? "Yes" : "No") << std::endl;
        std::cout << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    // Print final statistics
    std::cout << "\n=== Final Statistics ===" << std::endl;
    std::cout << "Library Version: " << WindVane::WindVane::getVersion() << std::endl;
    std::cout << "Build Info: " << WindVane::WindVane::getBuildInfo() << std::endl;
    
    CalibrationData data = windVane->getCalibrationData();
    std::cout << "Calibration Points: " << data.points.size() << std::endl;
    std::cout << "Calibration Valid: " << (data.isValid ? "Yes" : "No") << std::endl;
    
    std::cout << "\nSimulation completed successfully!" << std::endl;
    
    return 0;
}