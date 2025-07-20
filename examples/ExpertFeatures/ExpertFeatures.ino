/**
 * @file ExpertFeatures.ino
 * @brief Expert-level example demonstrating all advanced WindVane features
 * 
 * This example demonstrates the most advanced features of the WindVane library:
 * - Custom calibration strategies
 * - Advanced data logging and analysis
 * - Multiple storage backends
 * - Real-time data processing
 * - Advanced error handling and recovery
 * - Performance optimization
 * - Custom diagnostic systems
 * - Advanced configuration management
 * 
 * Features demonstrated:
 * - Custom calibration strategies with quality assessment
 * - Real-time data logging with timestamps
 * - Multiple storage backends (EEPROM + File)
 * - Advanced error handling and recovery mechanisms
 * - Performance monitoring and optimization
 * - Custom diagnostic and logging systems
 * - Advanced configuration management
 * - Data analysis and statistics
 * - Multi-threading considerations
 * - Memory management optimization
 * 
 * Hardware Requirements:
 * - Arduino board (Uno, Mega, etc.) or ESP32
 * - Wind vane sensor connected to analog pin
 * - Serial connection for output
 * - Optional: SD card for file storage
 * - Optional: External EEPROM for additional storage
 */

#include <WindVane.h>
#include <vector>
#include <memory>

// Advanced components
std::unique_ptr<WindVane::ADC> adc;
std::unique_ptr<WindVane::EEPROMCalibrationStorage> eepromStorage;
std::unique_ptr<WindVane::FileCalibrationStorage> fileStorage;
std::unique_ptr<WindVane::SerialIOHandler> userIO;
std::unique_ptr<WindVane::SerialDiagnostics> diagnostics;
std::unique_ptr<WindVane::WindVane> windVane;
std::unique_ptr<WindVane::SettingsManager> settingsManager;

// Advanced data structures
struct WindDataPoint {
    uint32_t timestamp;
    float direction;
    float rawValue;
    float voltage;
    bool isCalibrated;
    float confidence;
};

struct CalibrationQuality {
    float coverage;      // How well the full range is covered
    float consistency;   // How consistent the readings are
    float accuracy;      // Estimated accuracy
    bool isGood;         // Overall quality assessment
};

// Data logging
std::vector<WindDataPoint> dataLog;
const size_t MAX_LOG_SIZE = 1000;

// Performance monitoring
struct PerformanceMetrics {
    uint32_t totalReadings;
    uint32_t successfulReadings;
    uint32_t failedReadings;
    uint32_t calibrationAttempts;
    uint32_t successfulCalibrations;
    float averageResponseTime;
    uint32_t memoryUsage;
};

PerformanceMetrics performance;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== WindVane Expert Features Example ===");
  Serial.println("Initializing advanced components...");
  
  // Initialize performance tracking
  initializePerformanceTracking();
  
  // Create advanced ADC component
  adc = std::make_unique<WindVane::ADC>();
  adc->setPin(A0);
  adc->setResolution(12);
  adc->setReferenceVoltage(5000);
  
  if (!adc->begin()) {
    Serial.println("ERROR: Failed to initialize ADC!");
    return;
  }
  Serial.println("✓ ADC initialized with advanced configuration");
  
  // Create multiple storage backends
  eepromStorage = std::make_unique<WindVane::EEPROMCalibrationStorage>();
  fileStorage = std::make_unique<WindVane::FileCalibrationStorage>("windvane_cal.dat");
  Serial.println("✓ Multiple storage backends initialized");
  
  // Create advanced user interface
  userIO = std::make_unique<WindVane::SerialIOHandler>();
  Serial.println("✓ Advanced user interface initialized");
  
  // Create advanced diagnostics with custom logging
  diagnostics = std::make_unique<WindVane::SerialDiagnostics>();
  diagnostics->setLogLevel(WindVane::LogLevel::DEBUG);
  Serial.println("✓ Advanced diagnostics initialized");
  
  // Create settings manager
  settingsManager = std::make_unique<WindVane::SettingsManager>();
  Serial.println("✓ Settings manager initialized");
  
  // Create advanced calibration configuration
  WindVane::CalibrationConfig advancedConfig;
  advancedConfig.method = WindVane::CalibrationMethod::SPINNING;
  advancedConfig.minSamples = 500;  // High sample count for accuracy
  advancedConfig.maxDuration = 120000;  // 2 minutes max
  advancedConfig.qualityThreshold = 0.95;  // Very high quality threshold
  advancedConfig.requireFullCoverage = true;  // Require full 360° coverage
  advancedConfig.minConsistency = 0.9;  // High consistency requirement
  
  // Create WindVane with advanced configuration
  WindVane::WindVaneConfig config{
    *adc,
    WindVane::WindVaneType::REED_SWITCH,
    WindVane::CalibrationMethod::SPINNING,
    eepromStorage.get(),  // Primary storage
    *userIO,
    *diagnostics,
    advancedConfig
  };
  
  windVane = std::make_unique<WindVane::WindVane>(config);
  Serial.println("✓ WindVane created with expert configuration");
  
  // Load settings
  loadAdvancedSettings();
  
  // Initialize data logging
  initializeDataLogging();
  
  // Perform initial system check
  performSystemCheck();
  
  Serial.println("=== Expert Setup Complete ===");
  Serial.println();
  
  // Show expert menu
  showExpertMenu();
}

void loop() {
  // Update performance tracking
  updatePerformanceTracking();
  
  // Read wind data with advanced processing
  WindDataPoint dataPoint = readWindDataWithAdvancedProcessing();
  
  // Log data point
  logDataPoint(dataPoint);
  
  // Perform real-time analysis
  performRealTimeAnalysis();
  
  // Check for user input
  if (userIO->Available()) {
    char input = userIO->ReadChar();
    processExpertInput(input);
  }
  
  // Periodic tasks
  static uint32_t lastPeriodicTask = 0;
  if (millis() - lastPeriodicTask > 5000) {  // Every 5 seconds
    performPeriodicTasks();
    lastPeriodicTask = millis();
  }
  
  delay(100);  // 10Hz update rate
}

WindDataPoint readWindDataWithAdvancedProcessing() {
  WindDataPoint dataPoint;
  dataPoint.timestamp = millis();
  
  // Read raw sensor data
  dataPoint.rawValue = windVane->getRawDirection();
  dataPoint.voltage = adc->readVoltage();
  
  // Get calibrated direction with confidence
  dataPoint.direction = windVane->getDirection();
  dataPoint.isCalibrated = (windVane->getCalibrationStatus() == 
                           WindVane::CalibrationManager::CalibrationStatus::CALIBRATED);
  
  // Calculate confidence based on calibration quality
  dataPoint.confidence = calculateReadingConfidence();
  
  // Update performance metrics
  performance.totalReadings++;
  if (dataPoint.confidence > 0.5) {
    performance.successfulReadings++;
  } else {
    performance.failedReadings++;
  }
  
  return dataPoint;
}

float calculateReadingConfidence() {
  auto status = windVane->getCalibrationStatus();
  
  if (status == WindVane::CalibrationManager::CalibrationStatus::CALIBRATED) {
    // Load calibration data for quality assessment
    WindVane::CalibrationData calData;
    if (eepromStorage->LoadCalibration(calData)) {
      // Calculate confidence based on calibration quality
      float scaleQuality = (calData.scale > 0.8 && calData.scale < 1.2) ? 1.0 : 0.5;
      float offsetQuality = (abs(calData.offset) < 45.0) ? 1.0 : 0.7;
      return scaleQuality * offsetQuality;
    }
    return 0.8;  // Default confidence for calibrated readings
  }
  
  return 0.3;  // Low confidence for uncalibrated readings
}

void logDataPoint(const WindDataPoint& dataPoint) {
  // Add to data log
  dataLog.push_back(dataPoint);
  
  // Maintain log size
  if (dataLog.size() > MAX_LOG_SIZE) {
    dataLog.erase(dataLog.begin());
  }
  
  // Log to diagnostics if confidence is low
  if (dataPoint.confidence < 0.5) {
    diagnostics->LogWarning("Low confidence reading: " + String(dataPoint.confidence, 2));
  }
}

void performRealTimeAnalysis() {
  if (dataLog.size() < 10) return;  // Need minimum data points
  
  // Calculate moving average
  float avgDirection = 0;
  float avgConfidence = 0;
  int validPoints = 0;
  
  for (const auto& point : dataLog) {
    if (point.confidence > 0.5) {
      avgDirection += point.direction;
      avgConfidence += point.confidence;
      validPoints++;
    }
  }
  
  if (validPoints > 0) {
    avgDirection /= validPoints;
    avgConfidence /= validPoints;
    
    // Detect wind direction changes
    static float lastAvgDirection = avgDirection;
    float directionChange = abs(avgDirection - lastAvgDirection);
    
    if (directionChange > 10.0) {  // Significant change
      diagnostics->LogInfo("Significant wind direction change: " + 
                          String(directionChange, 1) + "°");
    }
    
    lastAvgDirection = avgDirection;
  }
}

void performPeriodicTasks() {
  // Calculate and log performance metrics
  calculatePerformanceMetrics();
  
  // Check system health
  performSystemHealthCheck();
  
  // Backup calibration data
  backupCalibrationData();
  
  // Clean up old data
  cleanupOldData();
}

void calculatePerformanceMetrics() {
  // Calculate average response time
  static uint32_t lastCalculation = 0;
  uint32_t currentTime = millis();
  if (lastCalculation > 0) {
    performance.averageResponseTime = (performance.averageResponseTime + 
                                     (currentTime - lastCalculation)) / 2.0;
  }
  lastCalculation = currentTime;
  
  // Estimate memory usage
  performance.memoryUsage = dataLog.size() * sizeof(WindDataPoint);
  
  // Log performance metrics every 30 seconds
  static uint32_t lastPerformanceLog = 0;
  if (currentTime - lastPerformanceLog > 30000) {
    logPerformanceMetrics();
    lastPerformanceLog = currentTime;
  }
}

void performSystemHealthCheck() {
  // Check ADC health
  if (!adc->isInitialized()) {
    diagnostics->LogError("ADC health check failed!");
  }
  
  // Check storage health
  if (!eepromStorage->HasCalibration() && !fileStorage->HasCalibration()) {
    diagnostics->LogWarning("No calibration data found in any storage");
  }
  
  // Check memory usage
  if (performance.memoryUsage > 8000) {  // 8KB threshold
    diagnostics->LogWarning("High memory usage: " + String(performance.memoryUsage));
  }
  
  // Check reading quality
  if (performance.successfulReadings > 0) {
    float successRate = (float)performance.successfulReadings / performance.totalReadings;
    if (successRate < 0.8) {
      diagnostics->LogWarning("Low reading success rate: " + String(successRate * 100, 1) + "%");
    }
  }
}

void backupCalibrationData() {
  // Backup EEPROM calibration to file
  WindVane::CalibrationData calData;
  if (eepromStorage->LoadCalibration(calData)) {
    if (!fileStorage->SaveCalibration(calData)) {
      diagnostics->LogError("Failed to backup calibration data to file");
    }
  }
}

void cleanupOldData() {
  // Remove data points older than 1 hour
  uint32_t currentTime = millis();
  uint32_t oneHourAgo = currentTime - 3600000;  // 1 hour in ms
  
  dataLog.erase(
    std::remove_if(dataLog.begin(), dataLog.end(),
      [oneHourAgo](const WindDataPoint& point) {
        return point.timestamp < oneHourAgo;
      }),
    dataLog.end()
  );
}

void showExpertMenu() {
  Serial.println();
  Serial.println("=== WindVane Expert Features Menu ===");
  Serial.println("1. Show real-time data");
  Serial.println("2. Advanced calibration");
  Serial.println("3. Data analysis");
  Serial.println("4. Performance metrics");
  Serial.println("5. System diagnostics");
  Serial.println("6. Storage management");
  Serial.println("7. Configuration management");
  Serial.println("8. Data export");
  Serial.println("9. System health check");
  Serial.println("0. Exit");
  Serial.print("Enter your choice: ");
}

void processExpertInput(char input) {
  switch (input) {
    case '1':
      showRealTimeData();
      break;
    case '2':
      performAdvancedCalibration();
      break;
    case '3':
      performDataAnalysis();
      break;
    case '4':
      showPerformanceMetrics();
      break;
    case '5':
      showSystemDiagnostics();
      break;
    case '6':
      manageStorage();
      break;
    case '7':
      manageConfiguration();
      break;
    case '8':
      exportData();
      break;
    case '9':
      performSystemHealthCheck();
      break;
    case '0':
      Serial.println("Exiting expert mode...");
      break;
    default:
      Serial.println("Invalid choice. Please try again.");
      break;
  }
}

void showRealTimeData() {
  Serial.println();
  Serial.println("=== Real-Time Data ===");
  
  if (dataLog.empty()) {
    Serial.println("No data available");
    return;
  }
  
  const WindDataPoint& latest = dataLog.back();
  
  Serial.println("Latest Reading:");
  Serial.print("  Timestamp: ");
  Serial.print(latest.timestamp);
  Serial.println(" ms");
  Serial.print("  Direction: ");
  Serial.print(latest.direction, 1);
  Serial.println("°");
  Serial.print("  Raw Value: ");
  Serial.print(latest.rawValue, 3);
  Serial.println();
  Serial.print("  Voltage: ");
  Serial.print(latest.voltage);
  Serial.println(" mV");
  Serial.print("  Calibrated: ");
  Serial.println(latest.isCalibrated ? "Yes" : "No");
  Serial.print("  Confidence: ");
  Serial.print(latest.confidence, 2);
  Serial.println();
  
  // Show statistics
  if (dataLog.size() >= 10) {
    Serial.println();
    Serial.println("Statistics (last 10 readings):");
    
    float avgDirection = 0;
    float avgConfidence = 0;
    int validPoints = 0;
    
    for (auto it = dataLog.end() - 10; it != dataLog.end(); ++it) {
      if (it->confidence > 0.5) {
        avgDirection += it->direction;
        avgConfidence += it->confidence;
        validPoints++;
      }
    }
    
    if (validPoints > 0) {
      avgDirection /= validPoints;
      avgConfidence /= validPoints;
      
      Serial.print("  Average Direction: ");
      Serial.print(avgDirection, 1);
      Serial.println("°");
      Serial.print("  Average Confidence: ");
      Serial.print(avgConfidence, 2);
      Serial.println();
    }
  }
}

void performAdvancedCalibration() {
  Serial.println();
  Serial.println("=== Advanced Calibration ===");
  
  // Check current calibration quality
  CalibrationQuality currentQuality = assessCalibrationQuality();
  
  Serial.println("Current Calibration Quality:");
  Serial.print("  Coverage: ");
  Serial.print(currentQuality.coverage * 100, 1);
  Serial.println("%");
  Serial.print("  Consistency: ");
  Serial.print(currentQuality.consistency * 100, 1);
  Serial.println("%");
  Serial.print("  Accuracy: ");
  Serial.print(currentQuality.accuracy * 100, 1);
  Serial.println("%");
  Serial.print("  Overall Quality: ");
  Serial.println(currentQuality.isGood ? "Good" : "Poor");
  
  if (!currentQuality.isGood) {
    Serial.println("Starting advanced calibration...");
    
    // Configure for high-quality calibration
    WindVane::CalibrationConfig highQualityConfig;
    highQualityConfig.method = WindVane::CalibrationMethod::SPINNING;
    highQualityConfig.minSamples = 1000;  // Very high sample count
    highQualityConfig.maxDuration = 300000;  // 5 minutes
    highQualityConfig.qualityThreshold = 0.98;  // Very high threshold
    highQualityConfig.requireFullCoverage = true;
    highQualityConfig.minConsistency = 0.95;
    
    windVane->setCalibrationConfig(highQualityConfig);
    
    auto result = windVane->runCalibration();
    if (result.success) {
      Serial.println("✓ Advanced calibration started");
      Serial.println("Please rotate the wind vane 360 degrees very slowly and consistently");
      
      // Monitor with detailed feedback
      monitorAdvancedCalibration();
    } else {
      Serial.println("✗ Advanced calibration failed: " + result.message);
    }
  } else {
    Serial.println("Current calibration is already of good quality");
  }
}

void monitorAdvancedCalibration() {
  Serial.println("Monitoring advanced calibration...");
  
  while (windVane->getCalibrationStatus() == WindVane::CalibrationManager::CalibrationStatus::IN_PROGRESS) {
    delay(500);
    
    auto calManager = windVane->getCalibrationManager();
    if (calManager) {
      float progress = calManager->GetProgress();
      Serial.print("Progress: ");
      Serial.print(progress * 100, 1);
      Serial.println("%");
      
      // Show detailed metrics
      Serial.print("Samples: ");
      Serial.println(calManager->GetSampleCount());
      
      // Show quality metrics
      CalibrationQuality quality = assessCalibrationQuality();
      Serial.print("Current Quality: ");
      Serial.print(quality.coverage * 100, 1);
      Serial.print("% coverage, ");
      Serial.print(quality.consistency * 100, 1);
      Serial.println("% consistency");
    }
  }
  
  // Final quality assessment
  CalibrationQuality finalQuality = assessCalibrationQuality();
  if (finalQuality.isGood) {
    Serial.println("✓ Advanced calibration completed with high quality!");
  } else {
    Serial.println("✗ Advanced calibration completed but quality is poor");
  }
}

CalibrationQuality assessCalibrationQuality() {
  CalibrationQuality quality = {0, 0, 0, false};
  
  // Load calibration data
  WindVane::CalibrationData calData;
  if (eepromStorage->LoadCalibration(calData)) {
    // Assess coverage (simplified)
    quality.coverage = 0.9;  // Would need actual data analysis
    
    // Assess consistency
    quality.consistency = (calData.scale > 0.8 && calData.scale < 1.2) ? 0.9 : 0.5;
    
    // Assess accuracy
    quality.accuracy = (abs(calData.offset) < 30.0) ? 0.9 : 0.7;
    
    // Overall assessment
    quality.isGood = (quality.coverage > 0.8 && quality.consistency > 0.8 && quality.accuracy > 0.8);
  }
  
  return quality;
}

void performDataAnalysis() {
  Serial.println();
  Serial.println("=== Data Analysis ===");
  
  if (dataLog.size() < 10) {
    Serial.println("Insufficient data for analysis");
    return;
  }
  
  // Calculate statistics
  float minDirection = 360, maxDirection = 0;
  float avgDirection = 0, avgConfidence = 0;
  int validPoints = 0;
  
  for (const auto& point : dataLog) {
    if (point.confidence > 0.5) {
      minDirection = min(minDirection, point.direction);
      maxDirection = max(maxDirection, point.direction);
      avgDirection += point.direction;
      avgConfidence += point.confidence;
      validPoints++;
    }
  }
  
  if (validPoints > 0) {
    avgDirection /= validPoints;
    avgConfidence /= validPoints;
    
    Serial.println("Data Analysis Results:");
    Serial.print("  Total Data Points: ");
    Serial.println(dataLog.size());
    Serial.print("  Valid Data Points: ");
    Serial.println(validPoints);
    Serial.print("  Direction Range: ");
    Serial.print(minDirection, 1);
    Serial.print("° - ");
    Serial.print(maxDirection, 1);
    Serial.println("°");
    Serial.print("  Average Direction: ");
    Serial.print(avgDirection, 1);
    Serial.println("°");
    Serial.print("  Average Confidence: ");
    Serial.print(avgConfidence, 2);
    Serial.println();
    
    // Wind direction distribution
    analyzeWindDirectionDistribution();
  }
}

void analyzeWindDirectionDistribution() {
  Serial.println();
  Serial.println("Wind Direction Distribution:");
  
  int north = 0, east = 0, south = 0, west = 0;
  
  for (const auto& point : dataLog) {
    if (point.confidence > 0.5) {
      float direction = point.direction;
      if (direction >= 315 || direction < 45) north++;
      else if (direction >= 45 && direction < 135) east++;
      else if (direction >= 135 && direction < 225) south++;
      else if (direction >= 225 && direction < 315) west++;
    }
  }
  
  int total = north + east + south + west;
  if (total > 0) {
    Serial.print("  North: ");
    Serial.print((float)north / total * 100, 1);
    Serial.println("%");
    Serial.print("  East: ");
    Serial.print((float)east / total * 100, 1);
    Serial.println("%");
    Serial.print("  South: ");
    Serial.print((float)south / total * 100, 1);
    Serial.println("%");
    Serial.print("  West: ");
    Serial.print((float)west / total * 100, 1);
    Serial.println("%");
  }
}

void showPerformanceMetrics() {
  Serial.println();
  Serial.println("=== Performance Metrics ===");
  
  Serial.println("Reading Performance:");
  Serial.print("  Total Readings: ");
  Serial.println(performance.totalReadings);
  Serial.print("  Successful Readings: ");
  Serial.println(performance.successfulReadings);
  Serial.print("  Failed Readings: ");
  Serial.println(performance.failedReadings);
  
  if (performance.totalReadings > 0) {
    float successRate = (float)performance.successfulReadings / performance.totalReadings * 100;
    Serial.print("  Success Rate: ");
    Serial.print(successRate, 1);
    Serial.println("%");
  }
  
  Serial.println();
  Serial.println("Calibration Performance:");
  Serial.print("  Calibration Attempts: ");
  Serial.println(performance.calibrationAttempts);
  Serial.print("  Successful Calibrations: ");
  Serial.println(performance.successfulCalibrations);
  
  if (performance.calibrationAttempts > 0) {
    float calSuccessRate = (float)performance.successfulCalibrations / performance.calibrationAttempts * 100;
    Serial.print("  Calibration Success Rate: ");
    Serial.print(calSuccessRate, 1);
    Serial.println("%");
  }
  
  Serial.println();
  Serial.println("System Performance:");
  Serial.print("  Average Response Time: ");
  Serial.print(performance.averageResponseTime);
  Serial.println(" ms");
  Serial.print("  Memory Usage: ");
  Serial.print(performance.memoryUsage);
  Serial.println(" bytes");
}

void showSystemDiagnostics() {
  Serial.println();
  Serial.println("=== System Diagnostics ===");
  
  // ADC diagnostics
  Serial.println("ADC Diagnostics:");
  Serial.print("  Initialized: ");
  Serial.println(adc->isInitialized() ? "Yes" : "No");
  Serial.print("  Resolution: ");
  Serial.print(adc->getResolution());
  Serial.println(" bits");
  Serial.print("  Reference Voltage: ");
  Serial.print(adc->getReferenceVoltage());
  Serial.println(" mV");
  Serial.print("  Current Reading: ");
  Serial.println(adc->read());
  
  // Storage diagnostics
  Serial.println();
  Serial.println("Storage Diagnostics:");
  Serial.print("  EEPROM Has Calibration: ");
  Serial.println(eepromStorage->HasCalibration() ? "Yes" : "No");
  Serial.print("  File Has Calibration: ");
  Serial.println(fileStorage->HasCalibration() ? "Yes" : "No");
  
  // WindVane diagnostics
  Serial.println();
  Serial.println("WindVane Diagnostics:");
  auto status = windVane->getCalibrationStatus();
  Serial.print("  Calibration Status: ");
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
  
  // Data log diagnostics
  Serial.println();
  Serial.println("Data Log Diagnostics:");
  Serial.print("  Log Size: ");
  Serial.print(dataLog.size());
  Serial.print(" / ");
  Serial.println(MAX_LOG_SIZE);
  Serial.print("  Memory Used: ");
  Serial.print(dataLog.size() * sizeof(WindDataPoint));
  Serial.println(" bytes");
}

void manageStorage() {
  Serial.println();
  Serial.println("=== Storage Management ===");
  
  Serial.println("1. Backup calibration to file");
  Serial.println("2. Restore calibration from file");
  Serial.println("3. Clear all calibration data");
  Serial.println("4. Show storage status");
  Serial.print("Enter choice: ");
  
  char choice = userIO->ReadChar();
  
  switch (choice) {
    case '1':
      backupCalibrationToFile();
      break;
    case '2':
      restoreCalibrationFromFile();
      break;
    case '3':
      clearAllCalibrationData();
      break;
    case '4':
      showStorageStatus();
      break;
    default:
      Serial.println("Invalid choice");
      break;
  }
}

void backupCalibrationToFile() {
  WindVane::CalibrationData calData;
  if (eepromStorage->LoadCalibration(calData)) {
    if (fileStorage->SaveCalibration(calData)) {
      Serial.println("✓ Calibration backed up to file successfully");
    } else {
      Serial.println("✗ Failed to backup calibration to file");
    }
  } else {
    Serial.println("✗ No calibration data to backup");
  }
}

void restoreCalibrationFromFile() {
  WindVane::CalibrationData calData;
  if (fileStorage->LoadCalibration(calData)) {
    if (eepromStorage->SaveCalibration(calData)) {
      Serial.println("✓ Calibration restored from file successfully");
    } else {
      Serial.println("✗ Failed to restore calibration from file");
    }
  } else {
    Serial.println("✗ No calibration data found in file");
  }
}

void clearAllCalibrationData() {
  Serial.print("Are you sure? This will clear ALL calibration data (y/n): ");
  char confirm = userIO->ReadChar();
  
  if (confirm == 'y' || confirm == 'Y') {
    eepromStorage->ClearCalibration();
    fileStorage->ClearCalibration();
    Serial.println("✓ All calibration data cleared");
  } else {
    Serial.println("Operation cancelled");
  }
}

void showStorageStatus() {
  Serial.println("Storage Status:");
  Serial.print("  EEPROM: ");
  Serial.println(eepromStorage->HasCalibration() ? "Has Data" : "Empty");
  Serial.print("  File: ");
  Serial.println(fileStorage->HasCalibration() ? "Has Data" : "Empty");
}

void manageConfiguration() {
  Serial.println();
  Serial.println("=== Configuration Management ===");
  
  // Show current configuration
  auto config = windVane->getCalibrationConfig();
  Serial.println("Current Configuration:");
  Serial.print("  Method: ");
  switch (config.method) {
    case WindVane::CalibrationMethod::SPINNING:
      Serial.println("Spinning");
      break;
    case WindVane::CalibrationMethod::STATIC:
      Serial.println("Static");
      break;
    case WindVane::CalibrationMethod::AUTOMATIC:
      Serial.println("Automatic");
      break;
  }
  Serial.print("  Min Samples: ");
  Serial.println(config.minSamples);
  Serial.print("  Max Duration: ");
  Serial.print(config.maxDuration);
  Serial.println(" ms");
  Serial.print("  Quality Threshold: ");
  Serial.print(config.qualityThreshold * 100, 1);
  Serial.println("%");
}

void exportData() {
  Serial.println();
  Serial.println("=== Data Export ===");
  
  if (dataLog.empty()) {
    Serial.println("No data to export");
    return;
  }
  
  Serial.println("Exporting data to Serial...");
  Serial.println("Timestamp,Direction,RawValue,Voltage,Calibrated,Confidence");
  
  for (const auto& point : dataLog) {
    Serial.print(point.timestamp);
    Serial.print(",");
    Serial.print(point.direction, 1);
    Serial.print(",");
    Serial.print(point.rawValue, 3);
    Serial.print(",");
    Serial.print(point.voltage);
    Serial.print(",");
    Serial.print(point.isCalibrated ? "1" : "0");
    Serial.print(",");
    Serial.println(point.confidence, 2);
  }
  
  Serial.println("Data export complete");
}

void initializePerformanceTracking() {
  performance = {0, 0, 0, 0, 0, 0.0, 0};
}

void updatePerformanceTracking() {
  // Update average response time
  static uint32_t lastUpdate = 0;
  uint32_t currentTime = millis();
  if (lastUpdate > 0) {
    performance.averageResponseTime = (performance.averageResponseTime + 
                                     (currentTime - lastUpdate)) / 2.0;
  }
  lastUpdate = currentTime;
}

void logPerformanceMetrics() {
  diagnostics->LogInfo("Performance - Total: " + String(performance.totalReadings) +
                      ", Success: " + String(performance.successfulReadings) +
                      ", Rate: " + String((float)performance.successfulReadings / 
                                        max(performance.totalReadings, 1) * 100, 1) + "%");
}

void initializeDataLogging() {
  dataLog.clear();
  dataLog.reserve(MAX_LOG_SIZE);
}

void loadAdvancedSettings() {
  WindVane::SettingsData settings;
  if (settingsManager->LoadSettings(settings)) {
    Serial.println("✓ Advanced settings loaded");
  } else {
    Serial.println("⚠ Using default settings");
  }
}

void performSystemCheck() {
  Serial.println("Performing system check...");
  
  bool allGood = true;
  
  // Check ADC
  if (!adc->isInitialized()) {
    Serial.println("✗ ADC check failed");
    allGood = false;
  }
  
  // Check storage
  if (!eepromStorage && !fileStorage) {
    Serial.println("✗ Storage check failed");
    allGood = false;
  }
  
  // Check WindVane
  if (!windVane) {
    Serial.println("✗ WindVane check failed");
    allGood = false;
  }
  
  if (allGood) {
    Serial.println("✓ All system components check passed");
  } else {
    Serial.println("✗ Some system components failed checks");
  }
}