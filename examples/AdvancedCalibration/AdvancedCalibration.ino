/**
 * @file AdvancedCalibration.ino
 * @brief Advanced calibration example for the WindVane library
 * 
 * This example demonstrates advanced calibration features including
 * multiple calibration methods, data validation, storage management,
 * and the complete menu system.
 */

#include <WindVane.h>

// Create a WindVane instance with advanced configuration
WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::esp32();
std::unique_ptr<WindVane::WindVane> windVane;

// Menu system components
std::unique_ptr<WindVane::IUserIO> userIO;
std::unique_ptr<WindVane::IOutput> output;
std::unique_ptr<WindVane::IDiagnostics> diagnostics;
std::unique_ptr<WindVane::ICalibrationStorage> storage;
std::unique_ptr<WindVane::IPlatform> platform;
std::unique_ptr<WindVane::SettingsManager> settingsManager;

// Menu system
std::unique_ptr<WindVane::WindVaneMenu> menu;

void setup() {
  Serial.begin(115200);
  Serial.println("WindVane Advanced Calibration Example");
  
  // Configure for ESP32 with advanced settings
  builder.setVaneType(WindVane::VaneType::POTENTIOMETER)
         .setCalibrationMethod(WindVane::CalibrationMethod::AUTOMATIC)
         .setADCConfig(WindVane::ADCConfig(36, 12, 3300, 8, 50))
         .setStorageConfig(WindVane::StorageConfig(WindVane::StorageType::FLASH, 0, 2048));
  
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
  
  // Initialize menu system components
  initializeMenuSystem();
  
  // Load existing calibration if available
  if (windVane->loadCalibration()) {
    Serial.println("Loaded existing calibration");
  } else {
    Serial.println("No calibration found, will need to calibrate");
  }
  
  // Show main menu
  if (menu) {
    menu->begin();
  }
}

void loop() {
  // Update the wind vane
  windVane->update();
  
  // Update the menu system
  if (menu) {
    menu->update();
  }
  
  // Handle serial input for direct commands
  if (Serial.available()) {
    char command = Serial.read();
    handleDirectCommand(command);
  }
  
  delay(100);
}

void initializeMenuSystem() {
  // Create platform-specific components
  platform = std::make_unique<WindVane::ArduinoPlatform>();
  
  // Create UI components
  userIO = std::make_unique<WindVane::SerialIOHandler>();
  output = std::make_unique<WindVane::SerialOutput>();
  
  // Create diagnostics
  diagnostics = std::make_unique<WindVane::BasicDiagnostics>(output.get());
  
  // Create storage
  storage = std::make_unique<WindVane::EEPROMCalibrationStorage>();
  
  // Create settings manager
  auto settingsStorage = std::make_unique<WindVane::EEPROMSettingsStorage>();
  settingsManager = std::make_unique<WindVane::SettingsManager>(*settingsStorage, *diagnostics);
  
  // Initialize components
  userIO->begin();
  output->begin();
  diagnostics->begin();
  storage->begin();
  settingsManager->begin();
  
  // Create menu configuration
  WindVane::WindVaneMenuConfig menuConfig{
    .vane = *windVane,
    .io = *userIO,
    .diag = *diagnostics,
    .bufferedDiag = std::nullopt,
    .out = *output,
    .storage = *storage,
    .settingsMgr = *settingsManager,
    .platform = *platform
  };
  
  // Create menu system
  menu = std::make_unique<WindVane::WindVaneMenu>(menuConfig);
  
  Serial.println("Menu system initialized");
}

void handleDirectCommand(char command) {
  switch (command) {
    case '1':
      startSpinningCalibration();
      break;
    case '2':
      startStaticCalibration();
      break;
    case '3':
      startManualCalibration();
      break;
    case '4':
      loadCalibration();
      break;
    case '5':
      saveCalibration();
      break;
    case '6':
      clearCalibration();
      break;
    case '7':
      validateCalibration();
      break;
    case '8':
      printCalibrationData();
      break;
    case '9':
      printMenu();
      break;
    case '0':
      printSystemInfo();
      break;
    case 'm':
    case 'M':
      showMenuSystem();
      break;
    default:
      break;
  }
}

void startSpinningCalibration() {
  Serial.println("Starting spinning calibration...");
  Serial.println("Please rotate the wind vane 360 degrees slowly and continuously.");
  
  if (windVane->startCalibration(WindVane::CalibrationMethod::SPINNING)) {
    Serial.println("Spinning calibration started. Rotate the wind vane now.");
  } else {
    Serial.println("Failed to start spinning calibration!");
  }
}

void startStaticCalibration() {
  Serial.println("Starting static calibration...");
  Serial.println("Please point the wind vane to known directions (0°, 90°, 180°, 270°).");
  
  if (windVane->startCalibration(WindVane::CalibrationMethod::STATIC)) {
    Serial.println("Static calibration started. Point to known directions.");
  } else {
    Serial.println("Failed to start static calibration!");
  }
}

void startManualCalibration() {
  Serial.println("Starting manual calibration...");
  Serial.println("Enter known wind directions when prompted.");
  
  if (windVane->startCalibration(WindVane::CalibrationMethod::MANUAL)) {
    Serial.println("Manual calibration started. Follow the prompts.");
  } else {
    Serial.println("Failed to start manual calibration!");
  }
}

void loadCalibration() {
  Serial.println("Loading calibration data...");
  if (windVane->loadCalibration()) {
    Serial.println("Calibration data loaded successfully!");
    printCalibrationData();
  } else {
    Serial.println("Failed to load calibration data!");
  }
}

void saveCalibration() {
  Serial.println("Saving calibration data...");
  if (windVane->saveCalibration()) {
    Serial.println("Calibration data saved successfully!");
  } else {
    Serial.println("Failed to save calibration data!");
  }
}

void clearCalibration() {
  Serial.println("Clearing calibration data...");
  if (windVane->clearCalibration()) {
    Serial.println("Calibration data cleared successfully!");
  } else {
    Serial.println("Failed to clear calibration data!");
  }
}

void validateCalibration() {
  Serial.println("Validating calibration data...");
  WindVane::CalibrationData data = windVane->getCalibrationData();
  
  if (data.isValid) {
    Serial.print("Calibration is valid with ");
    Serial.print(data.points.size());
    Serial.println(" points.");
    
    // Print calibration points
    for (size_t i = 0; i < data.points.size(); i++) {
      Serial.print("Point ");
      Serial.print(i);
      Serial.print(": Raw=");
      Serial.print(data.points[i].rawValue);
      Serial.print(", Direction=");
      Serial.print(data.points[i].direction.getDegrees());
      Serial.println("°");
    }
  } else {
    Serial.println("Calibration is not valid!");
  }
}

void printCalibrationData() {
  WindVane::CalibrationData data = windVane->getCalibrationData();
  
  Serial.println("=== Calibration Data ===");
  Serial.print("Valid: ");
  Serial.println(data.isValid ? "Yes" : "No");
  Serial.print("Points: ");
  Serial.println(data.points.size());
  Serial.print("Last calibration: ");
  Serial.print(data.lastCalibration.count());
  Serial.println(" ms ago");
}

void printSystemInfo() {
  Serial.println("=== System Information ===");
  Serial.print("Library version: ");
  Serial.println(WindVane::WindVane::getVersion());
  Serial.print("Build info: ");
  Serial.println(WindVane::WindVane::getBuildInfo());
  Serial.print("Platform: ");
  Serial.println(windVane->getConfig().type == WindVane::VaneType::POTENTIOMETER ? "Potentiometer" : "Reed Switch");
}

void showMenuSystem() {
  Serial.println("=== Menu System ===");
  Serial.println("The menu system provides an interactive interface.");
  Serial.println("Use the menu system for:");
  Serial.println("- Live wind direction display");
  Serial.println("- Interactive calibration");
  Serial.println("- Diagnostics and settings");
  Serial.println("- Help and documentation");
}

void printMenu() {
  Serial.println("\n=== WindVane Advanced Calibration Menu ===");
  Serial.println("1. Start Spinning Calibration");
  Serial.println("2. Start Static Calibration");
  Serial.println("3. Start Manual Calibration");
  Serial.println("4. Load Calibration");
  Serial.println("5. Save Calibration");
  Serial.println("6. Clear Calibration");
  Serial.println("7. Validate Calibration");
  Serial.println("8. Print Calibration Data");
  Serial.println("9. Show Menu");
  Serial.println("0. System Information");
  Serial.println("M. Show Menu System Info");
  Serial.println("Enter command (1-9, 0, M):");
}