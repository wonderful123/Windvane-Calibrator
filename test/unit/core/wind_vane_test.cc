#include "wind_vane/core/wind_vane.h"

#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "wind_vane/interfaces/analog_digital_converter.h"
#include "wind_vane/interfaces/calibration_storage.h"
#include "wind_vane/interfaces/diagnostics.h"
#include "wind_vane/interfaces/user_input_output.h"

namespace {

using ::testing::Return;
using ::testing::_;
using ::testing::StrictMock;

// Mock implementations for testing
class MockAnalogDigitalConverter : public AnalogDigitalConverter {
 public:
  MOCK_METHOD(float, ReadNormalized, (), (const, override));
  MOCK_METHOD(int, ReadRaw, (), (const, override));
  MOCK_METHOD(bool, IsAvailable, (), (const, override));
  MOCK_METHOD(int, GetResolutionBits, (), (const, override));
};

class MockCalibrationStorage : public CalibrationStorage {
 public:
  MOCK_METHOD(bool, Store, (const CalibrationData& data), (override));
  MOCK_METHOD(bool, Load, (CalibrationData* data), (override));
  MOCK_METHOD(bool, Clear, (), (override));
  MOCK_METHOD(uint64_t, GetLastTimestamp, (), (const, override));
};

class MockUserInputOutput : public UserInputOutput {
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

class MockDiagnostics : public Diagnostics {
 public:
  MOCK_METHOD(void, LogInfo, (const std::string& message), (override));
  MOCK_METHOD(void, LogWarning, (const std::string& message), (override));
  MOCK_METHOD(void, LogError, (const std::string& message), (override));
};

}  // namespace

class WindVaneTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Setup default mock behavior
    ON_CALL(mock_adc_, IsAvailable())
        .WillByDefault(Return(true));
    ON_CALL(mock_adc_, GetResolutionBits())
        .WillByDefault(Return(12));  // ESP32 default
    ON_CALL(mock_adc_, ReadNormalized())
        .WillByDefault(Return(0.5f));  // Mid-range reading
  }

  // Create WindVane with mocks
  std::unique_ptr<WindVane> CreateWindVane() {
    WindVane::Config config{
      .adc = mock_adc_,
      .storage = &mock_storage_,
      .user_io = mock_user_io_,
      .diagnostics = mock_diagnostics_
    };
    return std::make_unique<WindVane>(config);
  }

  StrictMock<MockAnalogDigitalConverter> mock_adc_;
  StrictMock<MockCalibrationStorage> mock_storage_;
  StrictMock<MockUserInputOutput> mock_user_io_;
  StrictMock<MockDiagnostics> mock_diagnostics_;
};

TEST_F(WindVaneTest, ConstructorInitializesSuccessfully) {
  EXPECT_CALL(mock_diagnostics_, LogInfo("WindVane initialized successfully"));
  
  auto wind_vane = CreateWindVane();
  EXPECT_NE(wind_vane, nullptr);
}

TEST_F(WindVaneTest, GetRawReadingReturnsAdcValue) {
  const float expected_reading = 0.75f;
  
  EXPECT_CALL(mock_adc_, IsAvailable())
      .WillOnce(Return(true));
  EXPECT_CALL(mock_adc_, ReadNormalized())
      .WillOnce(Return(expected_reading));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  float actual_reading = wind_vane->GetRawReading();
  
  EXPECT_FLOAT_EQ(actual_reading, expected_reading);
}

TEST_F(WindVaneTest, GetRawReadingHandlesUnavailableAdc) {
  EXPECT_CALL(mock_adc_, IsAvailable())
      .WillOnce(Return(false));
  EXPECT_CALL(mock_diagnostics_, LogError("ADC not available for reading"));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  float reading = wind_vane->GetRawReading();
  
  EXPECT_FLOAT_EQ(reading, 0.0f);
}

TEST_F(WindVaneTest, GetRawReadingClampsOutOfRangeValues) {
  const float out_of_range_reading = 1.5f;  // Above valid range
  const float expected_clamped = 1.0f;
  
  EXPECT_CALL(mock_adc_, IsAvailable())
      .WillOnce(Return(true));
  EXPECT_CALL(mock_adc_, ReadNormalized())
      .WillOnce(Return(out_of_range_reading));
  EXPECT_CALL(mock_diagnostics_, LogError(
      "ADC reading out of range: " + std::to_string(out_of_range_reading)));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  float reading = wind_vane->GetRawReading();
  
  EXPECT_FLOAT_EQ(reading, expected_clamped);
}

TEST_F(WindVaneTest, GetDirectionReturnsNormalizedAngleWhenNotCalibrated) {
  const float raw_reading = 0.25f;
  const float expected_angle = 90.0f;  // 0.25 * 360 = 90 degrees
  
  EXPECT_CALL(mock_adc_, IsAvailable())
      .WillOnce(Return(true));
  EXPECT_CALL(mock_adc_, ReadNormalized())
      .WillOnce(Return(raw_reading));
  EXPECT_CALL(mock_diagnostics_, LogWarning(
      "Wind vane not calibrated, returning raw reading"));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  float direction = wind_vane->GetDirection();
  
  EXPECT_FLOAT_EQ(direction, expected_angle);
}

TEST_F(WindVaneTest, ClearCalibrationCallsStorageClear) {
  EXPECT_CALL(mock_storage_, Clear())
      .WillOnce(Return(true));
  EXPECT_CALL(mock_diagnostics_, LogInfo("Calibration data cleared"));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  bool success = wind_vane->ClearCalibration();
  
  EXPECT_TRUE(success);
}

TEST_F(WindVaneTest, ClearCalibrationHandlesStorageFailure) {
  EXPECT_CALL(mock_storage_, Clear())
      .WillOnce(Return(false));
  EXPECT_CALL(mock_diagnostics_, LogError("Failed to clear calibration data"));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  bool success = wind_vane->ClearCalibration();
  
  EXPECT_FALSE(success);
}

TEST_F(WindVaneTest, GetLastCalibrationTimestampCallsStorage) {
  const uint64_t expected_timestamp = 1234567890ULL;
  
  EXPECT_CALL(mock_storage_, GetLastTimestamp())
      .WillOnce(Return(expected_timestamp));
  EXPECT_CALL(mock_diagnostics_, LogInfo(_));  // Constructor log
  
  auto wind_vane = CreateWindVane();
  uint64_t timestamp = wind_vane->GetLastCalibrationTimestamp();
  
  EXPECT_EQ(timestamp, expected_timestamp);
}