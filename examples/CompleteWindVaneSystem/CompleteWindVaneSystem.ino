/**
 * @file CompleteWindVaneSystem.ino
 * @brief Complete WindVane system example with all features
 * 
 * This example demonstrates the complete WindVane library including:
 * - Menu system with interactive UI
 * - Multiple calibration methods
 * - Storage management (EEPROM/Flash)
 * - Diagnostics and logging
 * - Platform abstraction
 * - Settings management
 * - Real-time wind direction display
 */

#include <WindVane.h>

// ============================================================================
// SYSTEM COMPONENTS
// ============================================================================

// Core WindVane instance
std::unique_ptr<WindVane::WindVane> windVane;

// Platform components
std::unique_ptr<WindVane::IPlatform> platform;
std::unique_ptr<WindVane::IADC> adc;

// UI components
std::unique_ptr<WindVane::IUserIO> userIO;
std::unique_ptr<WindVane::IOutput> output;

// Storage components
std::unique_ptr<WindVane::ICalibrationStorage> calibrationStorage;
std::unique_ptr<WindVane::ISettingsStorage> settingsStorage;

// Diagnostics components
std::unique_ptr<WindVane::IDiagnostics> diagnostics;
std::unique_ptr<WindVane::IBufferedDiagnostics> bufferedDiagnostics;

// Management components
std::unique_ptr<WindVane::SettingsManager> settingsManager;

// Menu system
std::unique_ptr<WindVane::WindVaneMenu> menu;

// ============================================================================
// CONFIGURATION
// ============================================================================

// System configuration
struct SystemConfig {
  static constexpr uint32_t SERIAL_BAUD = 115200;
  static constexpr uint8_t ADC_PIN = 36;  // ESP32 ADC1_CH0
  static constexpr uint8_t ADC_RESOLUTION = 12;
  static constexpr uint16_t ADC_REFERENCE_VOLTAGE = 3300; // mV
  static constexpr WindVane::VaneType VANE_TYPE = WindVane::VaneType::POTENTIOMETER;
  static constexpr WindVane::CalibrationMethod DEFAULT_CALIBRATION_METHOD = WindVane::CalibrationMethod::SPINNING;
  static constexpr WindVane::StorageType STORAGE_TYPE = WindVane::StorageType::FLASH;
};

// ============================================================================
// INITIALIZATION
// ============================================================================

void setup() {
  // Initialize serial communication
  Serial.begin(SystemConfig::SERIAL_BAUD);
  Serial.println("WindVane Complete System");
  Serial.println("========================");
  
  // Initialize all system components
  if (!initializeSystem()) {
    Serial.println("System initialization failed!");
    return;
  }
  
  Serial.println("System initialized successfully");
  
  // Load existing calibration
  if (windVane->loadCalibration()) {
    Serial.println("Loaded existing calibration");
  } else {
    Serial.println("No calibration found - will need to calibrate");
  }
  
  // Start the menu system
  if (menu) {
    menu->begin();
  }
  
  Serial.println("System ready!");
}

bool initializeSystem() {
  // Create platform-specific components
  platform = createPlatform();
  if (!platform) {
    Serial.println("Failed to create platform");
    return false;
  }
  
  // Create ADC
  adc = createADC();
  if (!adc) {
    Serial.println("Failed to create ADC");
    return false;
  }
  
  // Create UI components
  userIO = createUserIO();
  output = createOutput();
  if (!userIO || !output) {
    Serial.println("Failed to create UI components");
    return false;
  }
  
  // Create storage components
  calibrationStorage = createCalibrationStorage();
  settingsStorage = createSettingsStorage();
  if (!calibrationStorage || !settingsStorage) {
    Serial.println("Failed to create storage components");
    return false;
  }
  
  // Create diagnostics
  diagnostics = createDiagnostics();
  bufferedDiagnostics = createBufferedDiagnostics();
  if (!diagnostics) {
    Serial.println("Failed to create diagnostics");
    return false;
  }
  
  // Create settings manager
  settingsManager = std::make_unique<WindVane::SettingsManager>(*settingsStorage, *diagnostics);
  
  // Initialize all components
  if (!initializeComponents()) {
    Serial.println("Failed to initialize components");
    return false;
  }
  
  // Create WindVane instance
  windVane = createWindVane();
  if (!windVane) {
    Serial.println("Failed to create WindVane instance");
    return false;
  }
  
  // Create menu system
  menu = createMenu();
  if (!menu) {
    Serial.println("Failed to create menu system");
    return false;
  }
  
  return true;
}

std::unique_ptr<WindVane::IPlatform> createPlatform() {
  // Detect platform and create appropriate implementation
  #ifdef ESP32
    return std::make_unique<WindVane::ESP32Platform>();
  #else
    return std::make_unique<WindVane::ArduinoPlatform>();
  #endif
}

std::unique_ptr<WindVane::IADC> createADC() {
  // Create ADC with configuration
  auto adc = std::make_unique<WindVane::ESP32ADC>();
  adc->setPin(SystemConfig::ADC_PIN);
  adc->setResolution(SystemConfig::ADC_RESOLUTION);
  adc->setReferenceVoltage(SystemConfig::ADC_REFERENCE_VOLTAGE);
  return adc;
}

std::unique_ptr<WindVane::IUserIO> createUserIO() {
  return std::make_unique<WindVane::SerialIOHandler>();
}

std::unique_ptr<WindVane::IOutput> createOutput() {
  return std::make_unique<WindVane::SerialOutput>();
}

std::unique_ptr<WindVane::ICalibrationStorage> createCalibrationStorage() {
  switch (SystemConfig::STORAGE_TYPE) {
    case WindVane::StorageType::EEPROM:
      return std::make_unique<WindVane::EEPROMCalibrationStorage>();
    case WindVane::StorageType::FLASH:
      return std::make_unique<WindVane::FlashCalibrationStorage>();
    case WindVane::StorageType::FILE_SYSTEM:
      return std::make_unique<WindVane::FileCalibrationStorage>();
    default:
      return std::make_unique<WindVane::EEPROMCalibrationStorage>();
  }
}

std::unique_ptr<WindVane::ISettingsStorage> createSettingsStorage() {
  return std::make_unique<WindVane::EEPROMSettingsStorage>();
}

std::unique_ptr<WindVane::IDiagnostics> createDiagnostics() {
  return std::make_unique<WindVane::BasicDiagnostics>(output.get());
}

std::unique_ptr<WindVane::IBufferedDiagnostics> createBufferedDiagnostics() {
  return std::make_unique<WindVane::BufferedDiagnostics>(output.get());
}

std::unique_ptr<WindVane::WindVane> createWindVane() {
  // Configure WindVane
  WindVane::WindVaneConfig config;
  config.type = SystemConfig::VANE_TYPE;
  config.calibrationMethod = SystemConfig::DEFAULT_CALIBRATION_METHOD;
  config.minPoints = 8;
  config.maxPoints = 360;
  config.timeout = 300000; // 5 minutes
  
  // Create WindVane with all dependencies
  return std::make_unique<WindVane::WindVane>(
    config,
    std::move(adc),
    std::move(calibrationStorage),
    std::move(userIO),
    std::move(diagnostics)
  );
}

std::unique_ptr<WindVane::WindVaneMenu> createMenu() {
  // Create menu configuration
  WindVane::WindVaneMenuConfig menuConfig{
    .vane = *windVane,
    .io = *userIO,
    .diag = *diagnostics,
    .bufferedDiag = bufferedDiagnostics ? std::optional<std::reference_wrapper<WindVane::IBufferedDiagnostics>>(*bufferedDiagnostics) : std::nullopt,
    .out = *output,
    .storage = *calibrationStorage,
    .settingsMgr = *settingsManager,
    .platform = *platform
  };
  
  return std::make_unique<WindVane::WindVaneMenu>(menuConfig);
}

bool initializeComponents() {
  // Initialize all components
  if (!platform->begin()) {
    Serial.println("Failed to initialize platform");
    return false;
  }
  
  if (!adc->begin()) {
    Serial.println("Failed to initialize ADC");
    return false;
  }
  
  if (!userIO->begin()) {
    Serial.println("Failed to initialize user IO");
    return false;
  }
  
  if (!output->begin()) {
    Serial.println("Failed to initialize output");
    return false;
  }
  
  if (!calibrationStorage->begin()) {
    Serial.println("Failed to initialize calibration storage");
    return false;
  }
  
  if (!settingsStorage->begin()) {
    Serial.println("Failed to initialize settings storage");
    return false;
  }
  
  if (!diagnostics->begin()) {
    Serial.println("Failed to initialize diagnostics");
    return false;
  }
  
  if (!settingsManager->begin()) {
    Serial.println("Failed to initialize settings manager");
    return false;
  }
  
  if (!windVane->begin()) {
    Serial.println("Failed to initialize WindVane");
    return false;
  }
  
  return true;
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Update WindVane
  windVane->update();
  
  // Update menu system
  if (menu) {
    menu->update();
  }
  
  // Handle direct commands (for debugging)
  if (Serial.available()) {
    char command = Serial.read();
    handleDirectCommand(command);
  }
  
  // Update diagnostics
  if (diagnostics) {
    diagnostics->update();
  }
  
  // Small delay to prevent overwhelming the system
  delay(50);
}

// ============================================================================
// DIRECT COMMANDS (for debugging and testing)
// ============================================================================

void handleDirectCommand(char command) {
  switch (command) {
    case 'h':
    case 'H':
      showHelp();
      break;
    case 's':
    case 'S':
      showStatus();
      break;
    case 'c':
    case 'C':
      startCalibration();
      break;
    case 'd':
    case 'D':
      showDiagnostics();
      break;
    case 't':
    case 'T':
      runTests();
      break;
    case 'r':
    case 'R':
      resetSystem();
      break;
    default:
      break;
  }
}

void showHelp() {
  Serial.println("\n=== Direct Commands ===");
  Serial.println("H - Show this help");
  Serial.println("S - Show system status");
  Serial.println("C - Start calibration");
  Serial.println("D - Show diagnostics");
  Serial.println("T - Run system tests");
  Serial.println("R - Reset system");
  Serial.println("=====================");
}

void showStatus() {
  Serial.println("\n=== System Status ===");
  Serial.print("WindVane: ");
  Serial.println(windVane->isCalibrated() ? "Calibrated" : "Not calibrated");
  
  if (windVane->isCalibrated()) {
    WindVane::WindDirection direction = windVane->getDirection();
    Serial.print("Direction: ");
    Serial.print(direction.getDegrees());
    Serial.println("°");
    
    uint16_t rawValue = windVane->getRawValue();
    uint16_t voltage = windVane->getVoltage();
    Serial.print("Raw ADC: ");
    Serial.print(rawValue);
    Serial.print(", Voltage: ");
    Serial.print(voltage);
    Serial.println(" mV");
  }
  
  Serial.print("Platform: ");
  Serial.println(platform->getPlatformName());
  Serial.print("Storage: ");
  Serial.println(calibrationStorage->getStorageName());
  Serial.println("===================");
}

void startCalibration() {
  Serial.println("Starting calibration...");
  if (windVane->startCalibration()) {
    Serial.println("Calibration started. Follow the menu prompts.");
  } else {
    Serial.println("Failed to start calibration!");
  }
}

void showDiagnostics() {
  Serial.println("\n=== Diagnostics ===");
  if (diagnostics) {
    diagnostics->log("Manual diagnostics check", WindVane::StatusLevel::INFO);
  }
  Serial.println("Check the menu system for detailed diagnostics.");
  Serial.println("===================");
}

void runTests() {
  Serial.println("\n=== System Tests ===");
  
  // Test ADC
  if (adc) {
    uint16_t value = adc->read();
    Serial.print("ADC Test: ");
    Serial.println(value);
  }
  
  // Test storage
  if (calibrationStorage) {
    bool exists = calibrationStorage->exists();
    Serial.print("Storage Test: ");
    Serial.println(exists ? "Data exists" : "No data");
  }
  
  // Test platform
  if (platform) {
    WindVane::TimeMs time = platform->millis();
    Serial.print("Platform Test: ");
    Serial.print(time.count());
    Serial.println(" ms");
  }
  
  Serial.println("Tests completed");
  Serial.println("=================");
}

void resetSystem() {
  Serial.println("Resetting system...");
  
  // Clear calibration
  windVane->clearCalibration();
  
  // Reset settings
  if (settingsManager) {
    settingsManager->reset();
  }
  
  Serial.println("System reset complete");
}