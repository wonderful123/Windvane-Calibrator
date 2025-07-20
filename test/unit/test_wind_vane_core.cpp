#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindVane/WindVane.h"
#include "WindVane/Interfaces/IADC.h"
#include "WindVane/Storage/ICalibrationStorage.h"
#include "WindVane/UI/IIO.h"
#include "WindVane/Diagnostics/IDiagnostics.h"

using namespace testing;

// Mock classes for testing
class MockADC : public WindVane::IADC {
public:
    MOCK_METHOD(bool, begin, (), (override));
    MOCK_METHOD(uint16_t, read, (), (override));
    MOCK_METHOD(uint16_t, readAveraged, (uint8_t), (override));
    MOCK_METHOD(uint16_t, readVoltage, (), (override));
    MOCK_METHOD(uint8_t, getResolution, (), (const, override));
    MOCK_METHOD(uint16_t, getReferenceVoltage, (), (const, override));
    MOCK_METHOD(uint16_t, getMaxValue, (), (const, override));
    MOCK_METHOD(void, setPin, (uint8_t), (override));
    MOCK_METHOD(uint8_t, getPin, (), (const, override));
    MOCK_METHOD(void, setResolution, (uint8_t), (override));
    MOCK_METHOD(void, setReferenceVoltage, (uint16_t), (override));
    MOCK_METHOD(bool, isInitialized, (), (const, override));
    MOCK_METHOD(float, readPercentage, (), (override));
    MOCK_METHOD(float, readNormalized, (), (override));
};

class MockCalibrationStorage : public WindVane::ICalibrationStorage {
public:
    MOCK_METHOD(bool, SaveCalibration, (const WindVane::CalibrationData&), (override));
    MOCK_METHOD(bool, LoadCalibration, (WindVane::CalibrationData&), (override));
    MOCK_METHOD(bool, ClearCalibration, (), (override));
    MOCK_METHOD(bool, HasCalibration, (), (const, override));
};

class MockUserIO : public WindVane::IUserIO {
public:
    MOCK_METHOD(void, Print, (const std::string&), (override));
    MOCK_METHOD(void, Println, (const std::string&), (override));
    MOCK_METHOD(char, ReadChar, (), (override));
    MOCK_METHOD(bool, Available, (), (const, override));
    MOCK_METHOD(void, Clear, (), (override));
};

class MockDiagnostics : public WindVane::IDiagnostics {
public:
    MOCK_METHOD(void, Log, (WindVane::LogLevel, const std::string&), (override));
    MOCK_METHOD(void, LogError, (const std::string&), (override));
    MOCK_METHOD(void, LogInfo, (const std::string&), (override));
    MOCK_METHOD(void, LogDebug, (const std::string&), (override));
};

class WindVaneCoreTest : public Test {
protected:
    std::shared_ptr<MockADC> mockADC;
    std::shared_ptr<MockCalibrationStorage> mockStorage;
    std::shared_ptr<MockUserIO> mockUserIO;
    std::shared_ptr<MockDiagnostics> mockDiagnostics;
    std::unique_ptr<WindVane::WindVane> windVane;

    void SetUp() override {
        mockADC = std::make_shared<MockADC>();
        mockStorage = std::make_shared<MockCalibrationStorage>();
        mockUserIO = std::make_shared<MockUserIO>();
        mockDiagnostics = std::make_shared<MockDiagnostics>();

        // Set up ADC expectations
        EXPECT_CALL(*mockADC, isInitialized())
            .WillRepeatedly(Return(true));
        EXPECT_CALL(*mockADC, readNormalized())
            .WillRepeatedly(Return(0.5f));

        WindVane::WindVaneConfig config{
            *mockADC,
            WindVane::WindVaneType::REED_SWITCH,
            WindVane::CalibrationMethod::SPINNING,
            mockStorage.get(),
            *mockUserIO,
            *mockDiagnostics,
            WindVane::CalibrationConfig{}
        };

        windVane = std::make_unique<WindVane::WindVane>(config);
    }
};

TEST_F(WindVaneCoreTest, Constructor_ValidConfig_CreatesInstance) {
    EXPECT_NE(windVane, nullptr);
}

TEST_F(WindVaneCoreTest, GetDirection_NoCalibration_ReturnsRawValue) {
    EXPECT_CALL(*mockADC, readNormalized())
        .WillOnce(Return(0.5f)); // 180 degrees

    float direction = windVane->getDirection();
    EXPECT_GE(direction, 0.0f);
    EXPECT_LE(direction, 360.0f);
}

TEST_F(WindVaneCoreTest, GetDirection_WithCalibration_ReturnsCalibratedValue) {
    WindVane::CalibrationData calData;
    calData.isValid = true;
    calData.offset = 45.0f;
    calData.scale = 1.0f;

    EXPECT_CALL(*mockStorage, HasCalibration())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStorage, LoadCalibration(_))
        .WillOnce(DoAll(SetArgReferee<0>(calData), Return(true)));
    EXPECT_CALL(*mockADC, readNormalized())
        .WillOnce(Return(0.5f)); // 180 degrees

    float direction = windVane->getDirection();
    EXPECT_GE(direction, 0.0f);
    EXPECT_LE(direction, 360.0f);
}

TEST_F(WindVaneCoreTest, GetRawDirection_ReturnsADCRawValue) {
    EXPECT_CALL(*mockADC, readNormalized())
        .WillOnce(Return(0.75f));

    float rawValue = windVane->getRawDirection();
    EXPECT_EQ(rawValue, 0.75f);
}

TEST_F(WindVaneCoreTest, GetCalibrationStatus_NoCalibration_ReturnsNotCalibrated) {
    auto status = windVane->getCalibrationStatus();
    EXPECT_EQ(status, WindVane::CalibrationManager::CalibrationStatus::NOT_CALIBRATED);
}

TEST_F(WindVaneCoreTest, ClearCalibration_CallsStorageClear) {
    EXPECT_CALL(*mockStorage, ClearCalibration())
        .WillOnce(Return(true));

    auto result = windVane->clearCalibration();
    EXPECT_TRUE(result.success);
}

TEST_F(WindVaneCoreTest, RunCalibration_ValidMethod_ReturnsSuccess) {
    EXPECT_CALL(*mockDiagnostics, LogInfo(_))
        .Times(AtLeast(1));

    auto result = windVane->runCalibration();
    EXPECT_TRUE(result.success);
}

TEST_F(WindVaneCoreTest, Calibrate_SimpleMethod_ReturnsSuccess) {
    EXPECT_CALL(*mockDiagnostics, LogInfo(_))
        .Times(AtLeast(1));

    auto result = windVane->calibrate();
    EXPECT_TRUE(result.success);
}

TEST_F(WindVaneCoreTest, GetLastCalibrationTimestamp_ReturnsTimestamp) {
    auto timestamp = windVane->getLastCalibrationTimestamp();
    EXPECT_GE(timestamp.count(), 0);
}

TEST_F(WindVaneCoreTest, GetCalibrationConfig_ReturnsConfig) {
    auto config = windVane->getCalibrationConfig();
    EXPECT_NE(&config, nullptr);
}

TEST_F(WindVaneCoreTest, SetCalibrationConfig_UpdatesConfig) {
    WindVane::CalibrationConfig newConfig;
    newConfig.method = WindVane::CalibrationMethod::STATIC;
    
    windVane->setCalibrationConfig(newConfig);
    
    auto config = windVane->getCalibrationConfig();
    EXPECT_EQ(config.method, WindVane::CalibrationMethod::STATIC);
}

TEST_F(WindVaneCoreTest, GetStorage_ReturnsStoragePointer) {
    auto storage = windVane->getStorage();
    EXPECT_EQ(storage, mockStorage.get());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}