#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "WindVane/Storage/ICalibrationStorage.h"
#include "WindVane/Storage/EEPROMCalibrationStorage.h"
#include "WindVane/Storage/FileCalibrationStorage.h"
#include "WindVane/Storage/SettingsManager.h"
#include "WindVane/Storage/EEPROMSettingsStorage.h"
#include "WindVane/Storage/FileSettingsStorage.h"

using namespace testing;

class StorageSystemTest : public Test {
protected:
    void SetUp() override {
        // Set up test data
        testCalibrationData.isValid = true;
        testCalibrationData.offset = 45.0f;
        testCalibrationData.scale = 1.0f;
        testCalibrationData.timestamp = 1234567890;
        
        testSettingsData.version = 1;
        testSettingsData.updateInterval = 1000;
        testSettingsData.logLevel = WindVane::LogLevel::INFO;
    }

    void TearDown() override {
        // Clean up any test files
        std::remove("test_calibration.dat");
        std::remove("test_settings.dat");
        std::remove("corrupted_file.dat");
    }

    WindVane::CalibrationData testCalibrationData;
    WindVane::SettingsData testSettingsData;
};

TEST_F(StorageSystemTest, EEPROMCalibrationStorage_SaveAndLoad_WorksCorrectly) {
    WindVane::EEPROMCalibrationStorage storage;
    
    // Test save
    bool saveResult = storage.SaveCalibration(testCalibrationData);
    EXPECT_TRUE(saveResult);
    
    // Test load
    WindVane::CalibrationData loadedData;
    bool loadResult = storage.LoadCalibration(loadedData);
    EXPECT_TRUE(loadResult);
    
    // Verify data integrity
    EXPECT_EQ(loadedData.isValid, testCalibrationData.isValid);
    EXPECT_NEAR(loadedData.offset, testCalibrationData.offset, 0.01f);
    EXPECT_NEAR(loadedData.scale, testCalibrationData.scale, 0.01f);
    EXPECT_EQ(loadedData.timestamp, testCalibrationData.timestamp);
}

TEST_F(StorageSystemTest, EEPROMCalibrationStorage_HasCalibration_ReturnsCorrect) {
    WindVane::EEPROMCalibrationStorage storage;
    
    // Initially no calibration
    EXPECT_FALSE(storage.HasCalibration());
    
    // Save calibration
    storage.SaveCalibration(testCalibrationData);
    
    // Now has calibration
    EXPECT_TRUE(storage.HasCalibration());
}

TEST_F(StorageSystemTest, EEPROMCalibrationStorage_ClearCalibration_WorksCorrectly) {
    WindVane::EEPROMCalibrationStorage storage;
    
    // Save calibration
    storage.SaveCalibration(testCalibrationData);
    EXPECT_TRUE(storage.HasCalibration());
    
    // Clear calibration
    bool clearResult = storage.ClearCalibration();
    EXPECT_TRUE(clearResult);
    
    // Verify cleared
    EXPECT_FALSE(storage.HasCalibration());
}

TEST_F(StorageSystemTest, FileCalibrationStorage_SaveAndLoad_WorksCorrectly) {
    WindVane::FileCalibrationStorage storage("test_calibration.dat");
    
    // Test save
    bool saveResult = storage.SaveCalibration(testCalibrationData);
    EXPECT_TRUE(saveResult);
    
    // Test load
    WindVane::CalibrationData loadedData;
    bool loadResult = storage.LoadCalibration(loadedData);
    EXPECT_TRUE(loadResult);
    
    // Verify data integrity
    EXPECT_EQ(loadedData.isValid, testCalibrationData.isValid);
    EXPECT_NEAR(loadedData.offset, testCalibrationData.offset, 0.01f);
    EXPECT_NEAR(loadedData.scale, testCalibrationData.scale, 0.01f);
    EXPECT_EQ(loadedData.timestamp, testCalibrationData.timestamp);
}

TEST_F(StorageSystemTest, FileCalibrationStorage_HasCalibration_ReturnsCorrect) {
    WindVane::FileCalibrationStorage storage("test_calibration.dat");
    
    // Initially no calibration
    EXPECT_FALSE(storage.HasCalibration());
    
    // Save calibration
    storage.SaveCalibration(testCalibrationData);
    
    // Now has calibration
    EXPECT_TRUE(storage.HasCalibration());
}

TEST_F(StorageSystemTest, FileCalibrationStorage_ClearCalibration_WorksCorrectly) {
    WindVane::FileCalibrationStorage storage("test_calibration.dat");
    
    // Save calibration
    storage.SaveCalibration(testCalibrationData);
    EXPECT_TRUE(storage.HasCalibration());
    
    // Clear calibration
    bool clearResult = storage.ClearCalibration();
    EXPECT_TRUE(clearResult);
    
    // Verify cleared
    EXPECT_FALSE(storage.HasCalibration());
}

TEST_F(StorageSystemTest, EEPROMSettingsStorage_SaveAndLoad_WorksCorrectly) {
    WindVane::EEPROMSettingsStorage storage;
    
    // Test save
    bool saveResult = storage.SaveSettings(testSettingsData);
    EXPECT_TRUE(saveResult);
    
    // Test load
    WindVane::SettingsData loadedData;
    bool loadResult = storage.LoadSettings(loadedData);
    EXPECT_TRUE(loadResult);
    
    // Verify data integrity
    EXPECT_EQ(loadedData.version, testSettingsData.version);
    EXPECT_EQ(loadedData.updateInterval, testSettingsData.updateInterval);
    EXPECT_EQ(loadedData.logLevel, testSettingsData.logLevel);
}

TEST_F(StorageSystemTest, FileSettingsStorage_SaveAndLoad_WorksCorrectly) {
    WindVane::FileSettingsStorage storage("test_settings.dat");
    
    // Test save
    bool saveResult = storage.SaveSettings(testSettingsData);
    EXPECT_TRUE(saveResult);
    
    // Test load
    WindVane::SettingsData loadedData;
    bool loadResult = storage.LoadSettings(loadedData);
    EXPECT_TRUE(loadResult);
    
    // Verify data integrity
    EXPECT_EQ(loadedData.version, testSettingsData.version);
    EXPECT_EQ(loadedData.updateInterval, testSettingsData.updateInterval);
    EXPECT_EQ(loadedData.logLevel, testSettingsData.logLevel);
}

TEST_F(StorageSystemTest, SettingsManager_LoadAndSaveSettings_WorksCorrectly) {
    WindVane::SettingsManager manager;
    
    // Test save settings
    bool saveResult = manager.SaveSettings(testSettingsData);
    EXPECT_TRUE(saveResult);
    
    // Test load settings
    WindVane::SettingsData loadedData;
    bool loadResult = manager.LoadSettings(loadedData);
    EXPECT_TRUE(loadResult);
    
    // Verify data integrity
    EXPECT_EQ(loadedData.version, testSettingsData.version);
    EXPECT_EQ(loadedData.updateInterval, testSettingsData.updateInterval);
    EXPECT_EQ(loadedData.logLevel, testSettingsData.logLevel);
}

TEST_F(StorageSystemTest, SettingsManager_GetDefaultSettings_ReturnsValidSettings) {
    WindVane::SettingsManager manager;
    
    WindVane::SettingsData defaultSettings = manager.GetDefaultSettings();
    
    EXPECT_GT(defaultSettings.version, 0);
    EXPECT_GT(defaultSettings.updateInterval, 0);
    EXPECT_LE(defaultSettings.logLevel, WindVane::LogLevel::DEBUG);
    EXPECT_GE(defaultSettings.logLevel, WindVane::LogLevel::ERROR);
}

TEST_F(StorageSystemTest, SettingsManager_ResetToDefaults_WorksCorrectly) {
    WindVane::SettingsManager manager;
    
    // Save custom settings
    manager.SaveSettings(testSettingsData);
    
    // Reset to defaults
    bool resetResult = manager.ResetToDefaults();
    EXPECT_TRUE(resetResult);
    
    // Load settings and verify they're defaults
    WindVane::SettingsData loadedData;
    manager.LoadSettings(loadedData);
    
    WindVane::SettingsData defaultSettings = manager.GetDefaultSettings();
    EXPECT_EQ(loadedData.version, defaultSettings.version);
    EXPECT_EQ(loadedData.updateInterval, defaultSettings.updateInterval);
    EXPECT_EQ(loadedData.logLevel, defaultSettings.logLevel);
}

TEST_F(StorageSystemTest, StorageErrorHandling_InvalidData_HandlesGracefully) {
    WindVane::FileCalibrationStorage storage("nonexistent_file.dat");
    
    // Try to load from non-existent file
    WindVane::CalibrationData loadedData;
    bool loadResult = storage.LoadCalibration(loadedData);
    EXPECT_FALSE(loadResult);
    
    // Verify no calibration exists
    EXPECT_FALSE(storage.HasCalibration());
}

TEST_F(StorageSystemTest, StorageErrorHandling_CorruptedData_HandlesGracefully) {
    WindVane::FileCalibrationStorage storage("corrupted_file.dat");
    
    // Create a corrupted file
    std::ofstream file("corrupted_file.dat", std::ios::binary);
    file.write("corrupted data", 14);
    file.close();
    
    // Try to load corrupted data
    WindVane::CalibrationData loadedData;
    bool loadResult = storage.LoadCalibration(loadedData);
    EXPECT_FALSE(loadResult);
}

TEST_F(StorageSystemTest, StorageErrorHandling_InvalidSettingsFile_HandlesGracefully) {
    WindVane::FileSettingsStorage storage("nonexistent_settings.dat");
    
    // Try to load from non-existent file
    WindVane::SettingsData loadedData;
    bool loadResult = storage.LoadSettings(loadedData);
    EXPECT_FALSE(loadResult);
}

TEST_F(StorageSystemTest, StorageErrorHandling_CorruptedSettingsFile_HandlesGracefully) {
    WindVane::FileSettingsStorage storage("corrupted_settings.dat");
    
    // Create a corrupted file
    std::ofstream file("corrupted_settings.dat", std::ios::binary);
    file.write("corrupted settings", 18);
    file.close();
    
    // Try to load corrupted data
    WindVane::SettingsData loadedData;
    bool loadResult = storage.LoadSettings(loadedData);
    EXPECT_FALSE(loadResult);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}