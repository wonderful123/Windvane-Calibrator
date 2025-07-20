/**
 * @file BasicWindVane.ino
 * @brief Basic example of using the WindVane library
 * 
 * This example demonstrates the basic usage of the WindVane library
 * for reading wind direction and performing calibration.
 */

#include <WindVane.h>

// Create a WindVane instance using the builder pattern
WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::arduino();
std::unique_ptr<WindVane::WindVane> windVane;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("WindVane Basic Example");
  
  // Configure the wind vane
  builder.setVaneType(WindVane::VaneType::REED_SWITCH)
         .setCalibrationMethod(WindVane::CalibrationMethod::SPINNING)
         .setADCConfig(WindVane::ADCConfig(0, 12, 3300, 1, 100));
  
  // Build the wind vane instance
  windVane = builder.build();
  
  // Initialize the wind vane
  if (!windVane->begin()) {
    Serial.println("Failed to initialize wind vane!");
    return;
  }
  
  Serial.println("WindVane initialized successfully");
  
  // Load existing calibration if available
  if (windVane->loadCalibration()) {
    Serial.println("Loaded existing calibration");
  } else {
    Serial.println("No calibration found, will need to calibrate");
  }
}

void loop() {
  // Update the wind vane (call this regularly)
  windVane->update();
  
  // Check if calibration is needed
  if (!windVane->isCalibrated()) {
    Serial.println("Wind vane needs calibration!");
    Serial.println("Starting calibration...");
    
    if (windVane->startCalibration()) {
      Serial.println("Calibration started. Please rotate the wind vane 360 degrees.");
      
      // Wait for calibration to complete
      while (windVane->isCalibrating()) {
        windVane->update();
        
        uint8_t progress = windVane->getCalibrationProgress();
        Serial.print("Calibration progress: ");
        Serial.print(progress);
        Serial.println("%");
        
        delay(100);
      }
      
      if (windVane->stopCalibration()) {
        Serial.println("Calibration completed successfully!");
        windVane->saveCalibration();
      } else {
        Serial.println("Calibration failed!");
      }
    } else {
      Serial.println("Failed to start calibration!");
    }
  }
  
  // Get current wind direction
  WindVane::WindDirection direction = windVane->getDirection();
  uint16_t rawValue = windVane->getRawValue();
  uint16_t voltage = windVane->getVoltage();
  
  // Print wind information
  Serial.print("Wind Direction: ");
  Serial.print(direction.getDegrees());
  Serial.print("° (Raw: ");
  Serial.print(rawValue);
  Serial.print(", Voltage: ");
  Serial.print(voltage);
  Serial.println(" mV)");
  
  // Get complete measurement
  WindVane::WindMeasurement measurement = windVane->getMeasurement();
  if (measurement.isValid) {
    Serial.print("Measurement - Direction: ");
    Serial.print(measurement.direction.getDegrees());
    Serial.print("°, Speed: ");
    Serial.print(measurement.speed.getMPS());
    Serial.print(" m/s, Time: ");
    Serial.print(measurement.timestamp.count());
    Serial.println(" ms");
  }
  
  delay(1000);
}