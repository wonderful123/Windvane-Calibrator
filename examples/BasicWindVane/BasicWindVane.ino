/**
 * @file BasicWindVane.ino
 * @brief Basic example of using the WindVane library
 * 
 * This example demonstrates the basic usage of the WindVane library
 * for reading wind direction and performing simple calibration.
 * 
 * Features demonstrated:
 * - Basic wind direction reading
 * - Simple calibration process
 * - Raw sensor value reading
 * - Basic error handling
 * 
 * Hardware Requirements:
 * - Arduino board (Uno, Mega, etc.)
 * - Wind vane sensor connected to analog pin
 * - Serial connection for output
 */

#include <WindVane.h>

// Create components
std::unique_ptr<WindVane::ADC> adc;
std::unique_ptr<WindVane::EEPROMCalibrationStorage> storage;
std::unique_ptr<WindVane::SerialIOHandler> userIO;
std::unique_ptr<WindVane::SerialDiagnostics> diagnostics;
std::unique_ptr<WindVane::WindVane> windVane;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== WindVane Basic Example ===");
  Serial.println("Initializing components...");
  
  // Create ADC component
  adc = std::make_unique<WindVane::ADC>();
  adc->setPin(A0);  // Connect wind vane to A0
  adc->setResolution(10);  // 10-bit ADC for Arduino
  adc->setReferenceVoltage(5000);  // 5V reference
  
  if (!adc->begin()) {
    Serial.println("ERROR: Failed to initialize ADC!");
    return;
  }
  Serial.println("✓ ADC initialized");
  
  // Create storage component
  storage = std::make_unique<WindVane::EEPROMCalibrationStorage>();
  Serial.println("✓ Storage initialized");
  
  // Create user interface
  userIO = std::make_unique<WindVane::SerialIOHandler>();
  Serial.println("✓ User interface initialized");
  
  // Create diagnostics
  diagnostics = std::make_unique<WindVane::SerialDiagnostics>();
  Serial.println("✓ Diagnostics initialized");
  
  // Create WindVane configuration
  WindVane::WindVaneConfig config{
    *adc,
    WindVane::WindVaneType::REED_SWITCH,
    WindVane::CalibrationMethod::SPINNING,
    storage.get(),
    *userIO,
    *diagnostics,
    WindVane::CalibrationConfig{}
  };
  
  // Create WindVane instance
  windVane = std::make_unique<WindVane::WindVane>(config);
  Serial.println("✓ WindVane created");
  
  // Check if calibration exists
  if (windVane->getCalibrationStatus() == WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
    Serial.println("✓ Found existing calibration");
  } else {
    Serial.println("⚠ No calibration found - will need to calibrate");
  }
  
  Serial.println("=== Setup Complete ===");
  Serial.println();
}

void loop() {
  // Read wind direction
  float direction = windVane->getDirection();
  float rawValue = windVane->getRawDirection();
  
  // Get calibration status
  auto status = windVane->getCalibrationStatus();
  
  // Display current readings
  Serial.println("=== Current Readings ===");
  Serial.print("Wind Direction: ");
  Serial.print(direction, 1);
  Serial.println("°");
  
  Serial.print("Raw Sensor Value: ");
  Serial.print(rawValue, 3);
  Serial.println(" (0.0-1.0)");
  
  Serial.print("Calibration Status: ");
  switch (status) {
    case WindVane::CalibrationManager::CalibrationStatus::NOT_CALIBRATED:
      Serial.println("Not Calibrated");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS:
      Serial.println("Calibration In Progress");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::CALIBRATED:
      Serial.println("Calibrated");
      break;
  }
  
  // Check if calibration is needed
  if (status == WindVane::CalibrationManager::CalibrationStatus::NOT_CALIBRATED) {
    Serial.println();
    Serial.println("=== Calibration Required ===");
    Serial.println("Starting automatic calibration...");
    
    // Start calibration
    auto result = windVane->runCalibration();
    if (result.success) {
      Serial.println("✓ Calibration started successfully");
      Serial.println("Please rotate the wind vane 360 degrees slowly");
      
      // Monitor calibration progress
      while (windVane->getCalibrationStatus() == WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS) {
        delay(100);
        
        // Get progress from calibration manager
        auto calManager = windVane->getCalibrationManager();
        if (calManager) {
          float progress = calManager->GetProgress();
          Serial.print("Calibration Progress: ");
          Serial.print(progress * 100, 1);
          Serial.println("%");
        }
      }
      
      // Check final result
      if (windVane->getCalibrationStatus() == WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
        Serial.println("✓ Calibration completed successfully!");
      } else {
        Serial.println("✗ Calibration failed!");
      }
    } else {
      Serial.println("✗ Failed to start calibration: " + result.message);
    }
  }
  
  // Display additional sensor information
  Serial.println();
  Serial.println("=== Sensor Information ===");
  Serial.print("ADC Resolution: ");
  Serial.print(adc->getResolution());
  Serial.println(" bits");
  
  Serial.print("Reference Voltage: ");
  Serial.print(adc->getReferenceVoltage());
  Serial.println(" mV");
  
  Serial.print("Current Voltage: ");
  Serial.print(adc->readVoltage());
  Serial.println(" mV");
  
  Serial.print("ADC Raw Value: ");
  Serial.println(adc->read());
  
  Serial.print("Normalized Value: ");
  Serial.println(adc->readNormalized(), 3);
  
  Serial.println();
  Serial.println("=== End of Loop ===");
  Serial.println();
  
  delay(2000);  // Update every 2 seconds
}