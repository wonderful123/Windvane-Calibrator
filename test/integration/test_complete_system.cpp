#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindVane.h"
#include "WindVane/Core/WindVane.h"
#include "WindVane/Menu/WindVaneMenu.h"
#include "WindVane/Calibration/CalibrationManager.h"
#include "WindVane/Storage/EEPROMCalibrationStorage.h"
#include "WindVane/Storage/SettingsManager.h"
#include "WindVane/UI/SerialIOHandler.h"
#include "WindVane/Diagnostics/SerialDiagnostics.h"
#include "WindVane/Drivers/ADC.h"

using namespace testing;

class CompleteSystemTest : public Test {
protected:
    std::shared_ptr<WindVane::SerialIOHandler> userIO;
    std::shared_ptr<WindVane::SerialDiagnostics> diagnostics;
    std::shared_ptr<WindVane::ADC> adc;
    std::shared_ptr<WindVane::EEPROMCalibrationStorage> storage;
    std::shared_ptr<WindVane::SettingsManager> settingsManager;
    std::unique_ptr<WindVane::WindVane> windVane;
    std::unique_ptr<WindVane::WindVaneMenu> menu;

    void SetUp() override {
        // Create real components for integration testing
        userIO = std::make_shared<WindVane::SerialIOHandler>();
        diagnostics = std::make_shared<WindVane::SerialDiagnostics>();
        adc = std::make_shared<WindVane::ADC>();
        storage = std::make_shared<WindVane::EEPROMCalibrationStorage>();
        settingsManager = std::make_shared<WindVane::SettingsManager>();

        // Create WindVane with real components
        WindVane::WindVane::Config config{
            *adc,
            storage.get(),
            *userIO,
            *diagnostics
        };

        windVane = std::make_unique<WindVane::WindVane>(config);
        menu = std::make_unique<WindVane::WindVaneMenu>(
            userIO, diagnostics, windVane.get());
    }

    void TearDown() override {
        // Clean up any test data
        storage->ClearCalibration();
    }
};

TEST_F(CompleteSystemTest, SystemInitialization_AllComponents_WorkTogether) {
    // Test that all components can be created and work together
    EXPECT_NE(windVane, nullptr);
    EXPECT_NE(menu, nullptr);
    EXPECT_NE(userIO, nullptr);
    EXPECT_NE(diagnostics, nullptr);
    EXPECT_NE(adc, nullptr);
    EXPECT_NE(storage, nullptr);
    EXPECT_NE(settingsManager, nullptr);
}

TEST_F(CompleteSystemTest, WindVane_GetDirection_ReturnsValidValue) {
    // Test that wind vane can get direction readings
    float direction = windVane->GetDirection();
    EXPECT_GE(direction, 0.0f);
    EXPECT_LE(direction, 360.0f);
}

TEST_F(CompleteSystemTest, WindVane_GetRawReading_ReturnsValidValue) {
    // Test that wind vane can get raw readings
    float rawValue = windVane->GetRawReading();
    EXPECT_GE(rawValue, 0.0f);
    EXPECT_LE(rawValue, 1.0f);
}

TEST_F(CompleteSystemTest, WindVane_IsCalibrated_InitiallyFalse) {
    // Test that wind vane is not calibrated initially
    bool isCalibrated = windVane->IsCalibrated();
    EXPECT_FALSE(isCalibrated);
}

TEST_F(CompleteSystemTest, Storage_SaveAndLoadCalibration_WorksEndToEnd) {
    // Test complete calibration storage workflow
    WindVane::CalibrationData testData;
    testData.isValid = true;
    testData.offset = 30.0f;
    testData.scale = 1.0f;
    testData.timestamp = 1234567890;

    // Save calibration
    bool saveResult = storage->SaveCalibration(testData);
    EXPECT_TRUE(saveResult);

    // Verify storage has calibration
    EXPECT_TRUE(storage->HasCalibration());

    // Load calibration
    WindVane::CalibrationData loadedData;
    bool loadResult = storage->LoadCalibration(loadedData);
    EXPECT_TRUE(loadResult);

    // Verify data integrity
    EXPECT_EQ(loadedData.isValid, testData.isValid);
    EXPECT_NEAR(loadedData.offset, testData.offset, 0.01f);
    EXPECT_NEAR(loadedData.scale, testData.scale, 0.01f);
    EXPECT_EQ(loadedData.timestamp, testData.timestamp);
}

TEST_F(CompleteSystemTest, SettingsManager_SaveAndLoadSettings_WorksEndToEnd) {
    // Test complete settings management workflow
    WindVane::SettingsData testSettings;
    testSettings.version = 2;
    testSettings.updateInterval = 2000;
    testSettings.logLevel = WindVane::LogLevel::DEBUG;

    // Save settings
    bool saveResult = settingsManager->SaveSettings(testSettings);
    EXPECT_TRUE(saveResult);

    // Load settings
    WindVane::SettingsData loadedSettings;
    bool loadResult = settingsManager->LoadSettings(loadedSettings);
    EXPECT_TRUE(loadResult);

    // Verify data integrity
    EXPECT_EQ(loadedSettings.version, testSettings.version);
    EXPECT_EQ(loadedSettings.updateInterval, testSettings.updateInterval);
    EXPECT_EQ(loadedSettings.logLevel, testSettings.logLevel);
}

TEST_F(CompleteSystemTest, Menu_ShowMainMenu_DisplaysCorrectly) {
    // Test that menu can display main menu
    // This test verifies the menu system works with real components
    EXPECT_NO_THROW(menu->ShowMainMenu());
}

TEST_F(CompleteSystemTest, Menu_ShowDirectionDisplay_WorksWithRealWindVane) {
    // Test that menu can display direction with real wind vane
    EXPECT_NO_THROW(menu->ShowDirectionDisplay());
}

TEST_F(CompleteSystemTest, Menu_ShowDiagnosticsMenu_WorksWithRealComponents) {
    // Test that menu can display diagnostics with real components
    EXPECT_NO_THROW(menu->ShowDiagnosticsMenu());
}

TEST_F(CompleteSystemTest, Diagnostics_Logging_WorksWithRealComponents) {
    // Test that diagnostics system works with real components
    EXPECT_NO_THROW(diagnostics->LogInfo("Test info message"));
    EXPECT_NO_THROW(diagnostics->LogError("Test error message"));
    EXPECT_NO_THROW(diagnostics->LogDebug("Test debug message"));
}

TEST_F(CompleteSystemTest, UserIO_InputOutput_WorksWithRealComponents) {
    // Test that user IO system works with real components
    EXPECT_NO_THROW(userIO->Print("Test output"));
    EXPECT_NO_THROW(userIO->Println("Test output with newline"));
    EXPECT_NO_THROW(userIO->Clear());
}

TEST_F(CompleteSystemTest, ADC_Readings_WorksWithRealHardware) {
    // Test that ADC can read values (if hardware available)
    EXPECT_TRUE(adc->IsAvailable());
    
    float normalizedValue = adc->ReadNormalized();
    EXPECT_GE(normalizedValue, 0.0f);
    EXPECT_LE(normalizedValue, 1.0f);
    
    int rawValue = adc->ReadRaw();
    EXPECT_GE(rawValue, 0);
    
    int resolution = adc->GetResolutionBits();
    EXPECT_GT(resolution, 0);
}

TEST_F(CompleteSystemTest, Calibration_StartCalibration_WorksWithRealComponents) {
    // Test that calibration can be started with real components
    auto result = windVane->StartCalibration();
    EXPECT_TRUE(result.success);
}

TEST_F(CompleteSystemTest, System_ErrorHandling_WorksGracefully) {
    // Test that system handles errors gracefully
    // Try to get direction without calibration (should work)
    float direction = windVane->GetDirection();
    EXPECT_GE(direction, 0.0f);
    
    // Try to clear calibration when none exists (should work)
    bool clearResult = windVane->ClearCalibration();
    EXPECT_TRUE(clearResult);
}

TEST_F(CompleteSystemTest, System_Performance_ReasonableResponseTimes) {
    // Test that system responds within reasonable time
    auto start = std::chrono::high_resolution_clock::now();
    
    float direction = windVane->GetDirection();
    float rawValue = windVane->GetRawReading();
    bool isCalibrated = windVane->IsCalibrated();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete within 100ms
    EXPECT_LT(duration.count(), 100);
    
    // Verify we got valid results
    EXPECT_GE(direction, 0.0f);
    EXPECT_LE(direction, 360.0f);
    EXPECT_GE(rawValue, 0.0f);
    EXPECT_LE(rawValue, 1.0f);
    EXPECT_FALSE(isCalibrated); // Should be false initially
}

TEST_F(CompleteSystemTest, System_MemoryUsage_ReasonableFootprint) {
    // Test that system doesn't use excessive memory
    // This is a basic test - in a real system you'd use memory profiling tools
    
    // Create multiple instances to test memory management
    std::vector<std::unique_ptr<WindVane::WindVane>> windVanes;
    
    for (int i = 0; i < 5; ++i) {
        WindVane::WindVane::Config config{
            *adc,
            storage.get(),
            *userIO,
            *diagnostics
        };
        
        windVanes.push_back(std::make_unique<WindVane::WindVane>(config));
    }
    
    // All instances should work
    for (auto& wv : windVanes) {
        float direction = wv->GetDirection();
        EXPECT_GE(direction, 0.0f);
        EXPECT_LE(direction, 360.0f);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}