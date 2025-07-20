#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "../../../lib/Core/WindVane.h"
#include "../../../lib/Interfaces/IAnalogDigitalConverter.h"
#include "../../../lib/Interfaces/ICalibrationStorage.h"
#include "../../../lib/Interfaces/IDiagnostics.h"
#include "../../../lib/Interfaces/IUserInterface.h"

using ::testing::Return;
using ::testing::_;
using ::testing::StrictMock;
using ::testing::NiceMock;

// Mock implementations for testing following standard C++ naming
class MockAnalogDigitalConverter : public IAnalogDigitalConverter {
public:
    MOCK_METHOD(float, ReadNormalized, (), (const, override));
    MOCK_METHOD(int, ReadRaw, (), (const, override));
    MOCK_METHOD(bool, IsAvailable, (), (const, override));
    MOCK_METHOD(int, GetResolutionBits, (), (const, override));
    MOCK_METHOD(float, GetReferenceVoltage, (), (const, override));
};

class MockCalibrationStorage : public ICalibrationStorage {
public:
    MOCK_METHOD(bool, Store, (const CalibrationData& data), (override));
    MOCK_METHOD(bool, Load, (CalibrationData* data), (override));
    MOCK_METHOD(bool, Clear, (), (override));
    MOCK_METHOD(std::uint64_t, GetLastTimestamp, (), (const, override));
    MOCK_METHOD(bool, IsHealthy, (), (const, override));
};

class MockUserInterface : public IUserInterface {
public:
    MOCK_METHOD(bool, HasInput, (), (const, override));
    MOCK_METHOD(char, ReadCharacter, (), (const, override));
    MOCK_METHOD(void, FlushInput, (), (const, override));
    MOCK_METHOD(float, ReadFloat, (), (const, override));
    MOCK_METHOD(int, ReadInteger, (), (const, override));
    MOCK_METHOD(bool, ConfirmYesNo, (const std::string& prompt), (const, override));
    MOCK_METHOD(void, Write, (const std::string& text), (const, override));
    MOCK_METHOD(void, WriteLine, (const std::string& text), (const, override));
    MOCK_METHOD(void, Clear, (), (const, override));
    MOCK_METHOD(void, Wait, (int milliseconds), (const, override));
};

class MockDiagnostics : public IDiagnostics {
public:
    MOCK_METHOD(void, LogInfo, (const std::string& message), (override));
    MOCK_METHOD(void, LogWarning, (const std::string& message), (override));
    MOCK_METHOD(void, LogError, (const std::string& message), (override));
};

/**
 * @brief Test fixture for WindVane class following standard C++ conventions
 */
class WindVaneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup default mock behavior
        ON_CALL(m_mockAdc, IsAvailable())
            .WillByDefault(Return(true));
        ON_CALL(m_mockAdc, GetResolutionBits())
            .WillByDefault(Return(12));  // ESP32 default
        ON_CALL(m_mockAdc, GetReferenceVoltage())
            .WillByDefault(Return(3.3f));  // ESP32 default
        ON_CALL(m_mockAdc, ReadNormalized())
            .WillByDefault(Return(0.5f));  // Mid-range reading
        
        ON_CALL(m_mockStorage, IsHealthy())
            .WillByDefault(Return(true));
    }

    // Create WindVane instance with mocks
    std::unique_ptr<WindVane> CreateWindVane() {
        WindVane::Config config{
            .adc = m_mockAdc,
            .storage = &m_mockStorage,
            .userInterface = m_mockUserInterface,
            .diagnostics = m_mockDiagnostics
        };
        return std::make_unique<WindVane>(config);
    }

    // Mock objects following standard C++ naming with m_ prefix
    NiceMock<MockAnalogDigitalConverter> m_mockAdc;
    NiceMock<MockCalibrationStorage> m_mockStorage;
    NiceMock<MockUserInterface> m_mockUserInterface;
    NiceMock<MockDiagnostics> m_mockDiagnostics;
};

// Test cases following standard C++ naming: MethodName_Condition_ExpectedResult
TEST_F(WindVaneTest, Constructor_WithValidConfig_InitializesSuccessfully) {
    EXPECT_CALL(m_mockDiagnostics, LogInfo("WindVane initialized successfully"));
    
    auto windVane = CreateWindVane();
    EXPECT_NE(windVane, nullptr);
}

TEST_F(WindVaneTest, GetRawReading_WithValidAdc_ReturnsAdcValue) {
    const float expectedReading = 0.75f;
    
    EXPECT_CALL(m_mockAdc, IsAvailable())
        .WillOnce(Return(true));
    EXPECT_CALL(m_mockAdc, ReadNormalized())
        .WillOnce(Return(expectedReading));
    
    auto windVane = CreateWindVane();
    const float actualReading = windVane->GetRawReading();
    
    EXPECT_FLOAT_EQ(actualReading, expectedReading);
}

TEST_F(WindVaneTest, GetRawReading_WithUnavailableAdc_ReturnsZero) {
    EXPECT_CALL(m_mockAdc, IsAvailable())
        .WillOnce(Return(false));
    EXPECT_CALL(m_mockDiagnostics, LogError("ADC not available for reading"));
    
    auto windVane = CreateWindVane();
    const float reading = windVane->GetRawReading();
    
    EXPECT_FLOAT_EQ(reading, 0.0f);
}

TEST_F(WindVaneTest, GetRawReading_WithOutOfRangeValue_ClampsValue) {
    const float outOfRangeReading = 1.5f;  // Above valid range
    const float expectedClamped = 1.0f;
    
    EXPECT_CALL(m_mockAdc, IsAvailable())
        .WillOnce(Return(true));
    EXPECT_CALL(m_mockAdc, ReadNormalized())
        .WillOnce(Return(outOfRangeReading));
    EXPECT_CALL(m_mockDiagnostics, LogError(
        "ADC reading out of range: " + std::to_string(outOfRangeReading)));
    
    auto windVane = CreateWindVane();
    const float reading = windVane->GetRawReading();
    
    EXPECT_FLOAT_EQ(reading, expectedClamped);
}

TEST_F(WindVaneTest, GetDirection_WithoutCalibration_ReturnsNormalizedAngle) {
    const float rawReading = 0.25f;
    const float expectedAngle = 90.0f;  // 0.25 * 360 = 90 degrees
    
    EXPECT_CALL(m_mockAdc, IsAvailable())
        .WillOnce(Return(true));
    EXPECT_CALL(m_mockAdc, ReadNormalized())
        .WillOnce(Return(rawReading));
    EXPECT_CALL(m_mockDiagnostics, LogWarning(
        "Wind vane not calibrated, returning raw reading"));
    
    auto windVane = CreateWindVane();
    const float direction = windVane->GetDirection();
    
    EXPECT_FLOAT_EQ(direction, expectedAngle);
}

TEST_F(WindVaneTest, ClearCalibration_WithValidStorage_CallsStorageClear) {
    EXPECT_CALL(m_mockStorage, Clear())
        .WillOnce(Return(true));
    EXPECT_CALL(m_mockDiagnostics, LogInfo("Calibration data cleared"));
    
    auto windVane = CreateWindVane();
    const bool success = windVane->ClearCalibration();
    
    EXPECT_TRUE(success);
}

TEST_F(WindVaneTest, ClearCalibration_WithStorageFailure_ReturnsFalse) {
    EXPECT_CALL(m_mockStorage, Clear())
        .WillOnce(Return(false));
    EXPECT_CALL(m_mockDiagnostics, LogError("Failed to clear calibration data"));
    
    auto windVane = CreateWindVane();
    const bool success = windVane->ClearCalibration();
    
    EXPECT_FALSE(success);
}

TEST_F(WindVaneTest, GetLastCalibrationTimestamp_WithValidStorage_ReturnsTimestamp) {
    const std::uint64_t expectedTimestamp = 1234567890ULL;
    
    EXPECT_CALL(m_mockStorage, GetLastTimestamp())
        .WillOnce(Return(expectedTimestamp));
    
    auto windVane = CreateWindVane();
    const std::uint64_t timestamp = windVane->GetLastCalibrationTimestamp();
    
    EXPECT_EQ(timestamp, expectedTimestamp);
}

TEST_F(WindVaneTest, IsOperational_WithHealthyComponents_ReturnsTrue) {
    EXPECT_CALL(m_mockAdc, IsAvailable())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(m_mockAdc, ReadNormalized())
        .WillOnce(Return(0.5f));  // Valid reading
    EXPECT_CALL(m_mockStorage, IsHealthy())
        .WillOnce(Return(true));
    
    auto windVane = CreateWindVane();
    const bool operational = windVane->IsOperational();
    
    EXPECT_TRUE(operational);
}

TEST_F(WindVaneTest, IsOperational_WithUnhealthyAdc_ReturnsFalse) {
    EXPECT_CALL(m_mockAdc, IsAvailable())
        .WillOnce(Return(false));
    
    auto windVane = CreateWindVane();
    const bool operational = windVane->IsOperational();
    
    EXPECT_FALSE(operational);
}