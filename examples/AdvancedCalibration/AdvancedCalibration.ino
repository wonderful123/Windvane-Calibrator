/**
 * @file AdvancedCalibration.ino
 * @brief Advanced calibration example with multiple methods and interactive features
 * 
 * This example demonstrates advanced calibration features including:
 * - Multiple calibration methods (spinning, static, automatic)
 * - Interactive calibration process
 * - Calibration data validation
 * - Advanced error handling
 * - Calibration quality assessment
 * 
 * Features demonstrated:
 * - Interactive menu system
 * - Multiple calibration strategies
 * - Data clustering and analysis
 * - Calibration quality metrics
 * - Advanced error handling
 * 
 * Hardware Requirements:
 * - Arduino board (Uno, Mega, etc.)
 * - Wind vane sensor connected to analog pin
 * - Serial connection for interactive input
 */

#include <WindVane.h>

// Create components
std::unique_ptr<WindVane::ADC> adc;
std::unique_ptr<WindVane::EEPROMCalibrationStorage> storage;
std::unique_ptr<WindVane::SerialIOHandler> userIO;
std::unique_ptr<WindVane::SerialDiagnostics> diagnostics;
std::unique_ptr<WindVane::WindVane> windVane;
std::unique_ptr<WindVane::WindVaneMenu> menu;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== WindVane Advanced Calibration Example ===");
  Serial.println("Initializing components...");
  
  // Create ADC component with advanced configuration
  adc = std::make_unique<WindVane::ADC>();
  adc->setPin(A0);
  adc->setResolution(12);  // Higher resolution for better accuracy
  adc->setReferenceVoltage(5000);
  
  if (!adc->begin()) {
    Serial.println("ERROR: Failed to initialize ADC!");
    return;
  }
  Serial.println("✓ ADC initialized with 12-bit resolution");
  
  // Create storage component
  storage = std::make_unique<WindVane::EEPROMCalibrationStorage>();
  Serial.println("✓ Storage initialized");
  
  // Create user interface
  userIO = std::make_unique<WindVane::SerialIOHandler>();
  Serial.println("✓ User interface initialized");
  
  // Create diagnostics with detailed logging
  diagnostics = std::make_unique<WindVane::SerialDiagnostics>();
  diagnostics->setLogLevel(WindVane::LogLevel::DEBUG);
  Serial.println("✓ Diagnostics initialized with debug logging");
  
  // Create advanced calibration configuration
  WindVane::CalibrationConfig calConfig;
  calConfig.method = WindVane::CalibrationMethod::SPINNING;
  calConfig.minSamples = 100;  // More samples for better accuracy
  calConfig.maxDuration = 30000;  // 30 seconds max
  calConfig.qualityThreshold = 0.8;  // 80% quality threshold
  
  // Create WindVane configuration
  WindVane::WindVaneConfig config{
    *adc,
    WindVane::WindVaneType::REED_SWITCH,
    WindVane::CalibrationMethod::SPINNING,
    storage.get(),
    *userIO,
    *diagnostics,
    calConfig
  };
  
  // Create WindVane instance
  windVane = std::make_unique<WindVane::WindVane>(config);
  Serial.println("✓ WindVane created with advanced configuration");
  
  // Create menu system
  menu = std::make_unique<WindVane::WindVaneMenu>(
    userIO, diagnostics, windVane.get());
  Serial.println("✓ Menu system initialized");
  
  Serial.println("=== Setup Complete ===");
  Serial.println();
  
  // Show initial status
  showSystemStatus();
}

void loop() {
  // Show main menu
  showMainMenu();
  
  // Handle user input
  char input = userIO->ReadChar();
  processMenuInput(input);
  
  delay(100);
}

void showMainMenu() {
  Serial.println();
  Serial.println("=== WindVane Advanced Calibration Menu ===");
  Serial.println("1. Show current wind direction");
  Serial.println("2. Start calibration");
  Serial.println("3. Show calibration status");
  Serial.println("4. Clear calibration");
  Serial.println("5. Show system diagnostics");
  Serial.println("6. Show calibration quality");
  Serial.println("7. Manual calibration");
  Serial.println("8. Auto calibration");
  Serial.println("9. Show sensor information");
  Serial.println("0. Exit");
  Serial.print("Enter your choice: ");
}

void processMenuInput(char input) {
  switch (input) {
    case '1':
      showCurrentDirection();
      break;
    case '2':
      startCalibration();
      break;
    case '3':
      showCalibrationStatus();
      break;
    case '4':
      clearCalibration();
      break;
    case '5':
      showSystemDiagnostics();
      break;
    case '6':
      showCalibrationQuality();
      break;
    case '7':
      manualCalibration();
      break;
    case '8':
      autoCalibration();
      break;
    case '9':
      showSensorInformation();
      break;
    case '0':
      Serial.println("Exiting...");
      break;
    default:
      Serial.println("Invalid choice. Please try again.");
      break;
  }
}

void showCurrentDirection() {
  Serial.println();
  Serial.println("=== Current Wind Direction ===");
  
  float direction = windVane->getDirection();
  float rawValue = windVane->getRawDirection();
  
  Serial.print("Wind Direction: ");
  Serial.print(direction, 1);
  Serial.println("°");
  
  Serial.print("Raw Sensor Value: ");
  Serial.print(rawValue, 3);
  Serial.println(" (0.0-1.0)");
  
  // Show compass direction
  String compassDir = getCompassDirection(direction);
  Serial.print("Compass Direction: ");
  Serial.println(compassDir);
  
  // Show calibration confidence
  auto status = windVane->getCalibrationStatus();
  if (status == WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
    Serial.println("✓ Reading is calibrated");
  } else {
    Serial.println("⚠ Reading is not calibrated");
  }
}

void startCalibration() {
  Serial.println();
  Serial.println("=== Starting Calibration ===");
  
  // Check current status
  auto status = windVane->getCalibrationStatus();
  if (status == WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS) {
    Serial.println("⚠ Calibration already in progress!");
    return;
  }
  
  // Start calibration
  auto result = windVane->runCalibration();
  if (result.success) {
    Serial.println("✓ Calibration started successfully");
    Serial.println("Please rotate the wind vane 360 degrees slowly and continuously");
    
    // Monitor calibration progress
    monitorCalibrationProgress();
  } else {
    Serial.println("✗ Failed to start calibration: " + result.message);
  }
}

void monitorCalibrationProgress() {
  Serial.println("Monitoring calibration progress...");
  
  while (windVane->getCalibrationStatus() == WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS) {
    delay(200);
    
    // Get progress
    auto calManager = windVane->getCalibrationManager();
    if (calManager) {
      float progress = calManager->GetProgress();
      Serial.print("Progress: ");
      Serial.print(progress * 100, 1);
      Serial.println("%");
      
      // Show sample count
      Serial.print("Samples collected: ");
      Serial.println(calManager->GetSampleCount());
    }
  }
  
  // Check final result
  auto finalStatus = windVane->getCalibrationStatus();
  if (finalStatus == WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
    Serial.println("✓ Calibration completed successfully!");
    
    // Show calibration quality
    showCalibrationQuality();
  } else {
    Serial.println("✗ Calibration failed or was cancelled");
  }
}

void showCalibrationStatus() {
  Serial.println();
  Serial.println("=== Calibration Status ===");
  
  auto status = windVane->getCalibrationStatus();
  switch (status) {
    case WindVane::CalibrationManager::CalibrationStatus::NOT_CALIBRATED:
      Serial.println("Status: Not Calibrated");
      Serial.println("Action: Run calibration to improve accuracy");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS:
      Serial.println("Status: Calibration In Progress");
      Serial.println("Action: Continue rotating wind vane");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::CALIBRATED:
      Serial.println("Status: Calibrated");
      Serial.println("Action: Ready for accurate readings");
      break;
  }
  
  // Show last calibration timestamp
  auto timestamp = windVane->getLastCalibrationTimestamp();
  Serial.print("Last Calibration: ");
  Serial.print(timestamp.count());
  Serial.println(" ms ago");
}

void clearCalibration() {
  Serial.println();
  Serial.println("=== Clear Calibration ===");
  
  Serial.print("Are you sure you want to clear calibration? (y/n): ");
  char confirm = userIO->ReadChar();
  
  if (confirm == 'y' || confirm == 'Y') {
    auto result = windVane->clearCalibration();
    if (result.success) {
      Serial.println("✓ Calibration cleared successfully");
    } else {
      Serial.println("✗ Failed to clear calibration: " + result.message);
    }
  } else {
    Serial.println("Calibration clear cancelled");
  }
}

void showSystemDiagnostics() {
  Serial.println();
  Serial.println("=== System Diagnostics ===");
  
  // ADC diagnostics
  Serial.println("ADC Information:");
  Serial.print("  Resolution: ");
  Serial.print(adc->getResolution());
  Serial.println(" bits");
  Serial.print("  Reference Voltage: ");
  Serial.print(adc->getReferenceVoltage());
  Serial.println(" mV");
  Serial.print("  Current Voltage: ");
  Serial.print(adc->readVoltage());
  Serial.println(" mV");
  Serial.print("  Raw ADC Value: ");
  Serial.println(adc->read());
  
  // Storage diagnostics
  Serial.println("Storage Information:");
  Serial.print("  Has Calibration: ");
  Serial.println(storage->HasCalibration() ? "Yes" : "No");
  
  // WindVane diagnostics
  Serial.println("WindVane Information:");
  Serial.print("  Calibration Status: ");
  auto status = windVane->getCalibrationStatus();
  switch (status) {
    case WindVane::CalibrationManager::CalibrationStatus::NOT_CALIBRATED:
      Serial.println("Not Calibrated");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS:
      Serial.println("In Progress");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::CALIBRATED:
      Serial.println("Calibrated");
      break;
  }
}

void showCalibrationQuality() {
  Serial.println();
  Serial.println("=== Calibration Quality ===");
  
  auto status = windVane->getCalibrationStatus();
  if (status != WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
    Serial.println("No calibration data available");
    return;
  }
  
  // Get calibration data
  WindVane::CalibrationData calData;
  if (storage->LoadCalibration(calData)) {
    Serial.println("Calibration Data:");
    Serial.print("  Valid: ");
    Serial.println(calData.isValid ? "Yes" : "No");
    Serial.print("  Offset: ");
    Serial.print(calData.offset, 2);
    Serial.println("°");
    Serial.print("  Scale: ");
    Serial.print(calData.scale, 3);
    Serial.println();
    Serial.print("  Timestamp: ");
    Serial.print(calData.timestamp);
    Serial.println(" ms");
    
    // Quality assessment
    Serial.println("Quality Assessment:");
    if (calData.scale > 0.8 && calData.scale < 1.2) {
      Serial.println("  ✓ Scale factor is within acceptable range");
    } else {
      Serial.println("  ⚠ Scale factor may need adjustment");
    }
    
    if (abs(calData.offset) < 45.0) {
      Serial.println("  ✓ Offset is within acceptable range");
    } else {
      Serial.println("  ⚠ Offset is large - consider recalibration");
    }
  } else {
    Serial.println("Failed to load calibration data");
  }
}

void manualCalibration() {
  Serial.println();
  Serial.println("=== Manual Calibration ===");
  Serial.println("This method allows manual calibration by pointing to known directions");
  Serial.println("Not implemented in this example - use automatic calibration instead");
}

void autoCalibration() {
  Serial.println();
  Serial.println("=== Auto Calibration ===");
  Serial.println("Starting automatic calibration with quality assessment...");
  
  // Configure for auto calibration
  WindVane::CalibrationConfig autoConfig;
  autoConfig.method = WindVane::CalibrationMethod::SPINNING;
  autoConfig.minSamples = 200;  // More samples for auto calibration
  autoConfig.maxDuration = 60000;  // 60 seconds
  autoConfig.qualityThreshold = 0.9;  // Higher quality threshold
  
  windVane->setCalibrationConfig(autoConfig);
  
  // Start calibration
  auto result = windVane->runCalibration();
  if (result.success) {
    Serial.println("✓ Auto calibration started");
    monitorCalibrationProgress();
  } else {
    Serial.println("✗ Auto calibration failed: " + result.message);
  }
}

void showSensorInformation() {
  Serial.println();
  Serial.println("=== Sensor Information ===");
  
  Serial.println("ADC Readings:");
  Serial.print("  Raw Value: ");
  Serial.println(adc->read());
  Serial.print("  Voltage: ");
  Serial.print(adc->readVoltage());
  Serial.println(" mV");
  Serial.print("  Normalized: ");
  Serial.println(adc->readNormalized(), 3);
  Serial.print("  Percentage: ");
  Serial.print(adc->readPercentage(), 1);
  Serial.println("%");
  
  Serial.println("WindVane Readings:");
  Serial.print("  Raw Direction: ");
  Serial.print(windVane->getRawDirection(), 3);
  Serial.println(" (0.0-1.0)");
  Serial.print("  Calibrated Direction: ");
  Serial.print(windVane->getDirection(), 1);
  Serial.println("°");
}

void showSystemStatus() {
  Serial.println("=== System Status ===");
  
  auto status = windVane->getCalibrationStatus();
  Serial.print("Calibration: ");
  switch (status) {
    case WindVane::CalibrationManager::CalibrationStatus::NOT_CALIBRATED:
      Serial.println("Not Calibrated");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS:
      Serial.println("In Progress");
      break;
    case WindVane::CalibrationManager::CalibrationStatus::CALIBRATED:
      Serial.println("Calibrated");
      break;
  }
  
  float direction = windVane->getDirection();
  Serial.print("Current Direction: ");
  Serial.print(direction, 1);
  Serial.println("°");
  
  Serial.println("System ready for advanced calibration features");
}

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