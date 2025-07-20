/**
 * @file SimpleWindVane.ino
 * @brief Simple example for beginners - basic wind direction reading
 * 
 * This is the simplest possible example of using the WindVane library.
 * It shows how to:
 * - Set up basic wind direction reading
 * - Display wind direction in degrees
 * - Show compass direction (North, South, East, West)
 * 
 * This example is perfect for beginners who want to:
 * - Get started quickly
 * - Understand basic wind vane operation
 * - See immediate results
 * 
 * Hardware Requirements:
 * - Arduino board (Uno, Nano, etc.)
 * - Wind vane sensor connected to analog pin A0
 * - Serial connection for output
 */

#include <WindVane.h>

// Create simple components
WindVane::ADC adc;
WindVane::EEPROMCalibrationStorage storage;
WindVane::SerialIOHandler userIO;
WindVane::SerialDiagnostics diagnostics;
WindVane::WindVane windVane;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Simple WindVane Example");
  Serial.println("========================");
  
  // Set up ADC
  adc.setPin(A0);  // Connect wind vane to A0
  adc.setResolution(10);  // 10-bit for Arduino
  adc.setReferenceVoltage(5000);  // 5V reference
  
  if (!adc.begin()) {
    Serial.println("Error: Could not start ADC!");
    return;
  }
  
  // Create WindVane with simple configuration
  WindVane::WindVaneConfig config{
    adc,
    WindVane::WindVaneType::REED_SWITCH,
    WindVane::CalibrationMethod::SPINNING,
    &storage,
    userIO,
    diagnostics,
    WindVane::CalibrationConfig{}
  };
  
  windVane = WindVane::WindVane(config);
  
  Serial.println("WindVane ready!");
  Serial.println("Reading wind direction...");
  Serial.println();
}

void loop() {
  // Read wind direction
  float direction = windVane.getDirection();
  
  // Display the reading
  Serial.print("Wind Direction: ");
  Serial.print(direction, 1);
  Serial.print("° (");
  Serial.print(getCompassDirection(direction));
  Serial.println(")");
  
  // Show raw sensor value
  float rawValue = windVane.getRawDirection();
  Serial.print("Raw Sensor: ");
  Serial.print(rawValue, 3);
  Serial.println(" (0.0-1.0)");
  
  // Show if calibrated
  auto status = windVane.getCalibrationStatus();
  if (status == WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
    Serial.println("Status: Calibrated ✓");
  } else {
    Serial.println("Status: Not calibrated ⚠");
  }
  
  Serial.println("-------------------");
  
  delay(1000);  // Update every second
}

// Helper function to convert degrees to compass direction
String getCompassDirection(float degrees) {
  if (degrees >= 337.5 || degrees < 22.5) return "North";
  if (degrees >= 22.5 && degrees < 67.5) return "Northeast";
  if (degrees >= 67.5 && degrees < 112.5) return "East";
  if (degrees >= 112.5 && degrees < 157.5) return "Southeast";
  if (degrees >= 157.5 && degrees < 202.5) return "South";
  if (degrees >= 202.5 && degrees < 247.5) return "Southwest";
  if (degrees >= 247.5 && degrees < 292.5) return "West";
  if (degrees >= 292.5 && degrees < 337.5) return "Northwest";
  return "Unknown";
}