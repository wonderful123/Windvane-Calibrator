/**
 * @file HostSimulation.ino
 * @brief Host simulation example for the WindVane library
 * 
 * This example demonstrates using the WindVane library on a desktop
 * platform with simulated sensor data and file-based storage.
 * 
 * Note: This example is designed for native/desktop platforms.
 * For embedded platforms, use BasicWindVane.ino or AdvancedCalibration.ino
 */

#include <WindVane.h>

// Simulated wind vane data
class SimulatedWindVane {
private:
    float current_angle;
    float current_speed;
    unsigned long last_update;
    
public:
    SimulatedWindVane() 
        : current_angle(0.0f)
        , current_speed(5.0f)
        , last_update(0) {}
    
    void update() {
        unsigned long now = millis();
        if (now - last_update < 100) return; // Update every 100ms
        last_update = now;
        
        // Simulate gradual wind changes
        float angle_change = (random(-180, 180)) * 0.01f;
        current_angle += angle_change;
        if (current_angle < 0) current_angle += 360.0f;
        if (current_angle >= 360) current_angle -= 360.0f;
        
        float speed_change = (random(-100, 100)) * 0.02f;
        current_speed += speed_change;
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

// Global variables
SimulatedWindVane simulatedVane;
std::unique_ptr<WindVane::WindVane> windVane;
bool calibration_complete = false;
int simulation_step = 0;

void setup() {
    Serial.begin(115200);
    delay(1000); // Wait for serial to initialize
    
    Serial.println("WindVane Host Simulation Example");
    Serial.println("=================================");
    
    // Configure WindVane for host platform
    WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::host();
    builder.setVaneType(WindVane::VaneType::POTENTIOMETER)
           .setCalibrationMethod(WindVane::CalibrationMethod::AUTOMATIC)
           .setStorageConfig(WindVane::StorageConfig(WindVane::StorageType::FILE_SYSTEM, 0, 1024, "./calibration.dat"));
    
    // Build WindVane instance
    windVane = builder.build();
    
    if (!windVane) {
        Serial.println("Failed to create WindVane instance!");
        return;
    }
    
    // Initialize WindVane
    if (!windVane->begin()) {
        Serial.println("Failed to initialize WindVane!");
        return;
    }
    
    Serial.println("WindVane initialized successfully");
    
    // Load existing calibration if available
    if (windVane->loadCalibration()) {
        Serial.println("Loaded existing calibration");
        calibration_complete = true;
    } else {
        Serial.println("No calibration found, starting automatic calibration...");
        startCalibration();
    }
}

void loop() {
    // Update simulated wind
    simulatedVane.update();
    
    // Update WindVane
    windVane->update();
    
    // Handle calibration
    if (!calibration_complete) {
        handleCalibration();
        return;
    }
    
    // Main simulation loop
    if (simulation_step < 100) {
        // Get measurements
        WindVane::WindDirection direction = windVane->getDirection();
        uint16_t rawValue = windVane->getRawValue();
        uint16_t voltage = windVane->getVoltage();
        
        // Print measurements every second
        static unsigned long last_print = 0;
        if (millis() - last_print >= 1000) {
            Serial.print("Simulation Step ");
            Serial.print(simulation_step + 1);
            Serial.println(":");
            Serial.print("  Simulated Angle: ");
            Serial.print(simulatedVane.getAngle());
            Serial.println("°");
            Serial.print("  Measured Direction: ");
            Serial.print(direction.getDegrees());
            Serial.println("°");
            Serial.print("  Simulated Speed: ");
            Serial.print(simulatedVane.getSpeed());
            Serial.println(" m/s");
            Serial.print("  Raw ADC Value: ");
            Serial.println(rawValue);
            Serial.print("  Voltage: ");
            Serial.print(voltage);
            Serial.println(" mV");
            Serial.print("  Calibrated: ");
            Serial.println(windVane->isCalibrated() ? "Yes" : "No");
            Serial.println();
            
            last_print = millis();
            simulation_step++;
        }
    } else if (simulation_step == 100) {
        // Print final statistics
        Serial.println("=== Final Statistics ===");
        Serial.print("Library Version: ");
        Serial.println(WindVane::WindVane::getVersion());
        Serial.print("Build Info: ");
        Serial.println(WindVane::WindVane::getBuildInfo());
        
        WindVane::CalibrationData data = windVane->getCalibrationData();
        Serial.print("Calibration Points: ");
        Serial.println(data.points.size());
        Serial.print("Calibration Valid: ");
        Serial.println(data.isValid ? "Yes" : "No");
        
        Serial.println("Simulation completed successfully!");
        simulation_step++;
    }
    
    delay(50); // Small delay to prevent overwhelming
}

void startCalibration() {
    Serial.println("Starting automatic calibration...");
    Serial.println("Simulating wind vane rotation...");
    
    if (windVane->startCalibration(WindVane::CalibrationMethod::AUTOMATIC)) {
        // Calibration will be handled in handleCalibration()
    } else {
        Serial.println("Failed to start calibration!");
    }
}

void handleCalibration() {
    static int calibration_step = 0;
    static unsigned long last_calibration_update = 0;
    
    if (millis() - last_calibration_update < 50) return; // Update every 50ms
    last_calibration_update = millis();
    
    if (windVane->isCalibrating()) {
        // Simulate rotation through all angles
        float angle = (calibration_step / 100.0f) * 360.0f;
        simulatedVane.update();
        
        calibration_step++;
        
        // Show progress
        if (calibration_step % 20 == 0) {
            Serial.print("Calibration progress: ");
            Serial.print(windVane->getCalibrationProgress());
            Serial.println("%");
        }
    } else {
        // Calibration finished
        if (windVane->stopCalibration()) {
            Serial.println("Calibration completed successfully!");
            windVane->saveCalibration();
            calibration_complete = true;
        } else {
            Serial.println("Calibration failed!");
            calibration_complete = true;
        }
    }
}