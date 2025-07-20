#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindVane.h"
#include "WindVane/Core/WindVane.h"
#include "WindVane/Interfaces/IADC.h"
#include "WindVane/Interfaces/ICalibrationStorage.h"
#include "WindVane/Interfaces/IUserIO.h"
#include "WindVane/Interfaces/IDiagnostics.h"

using namespace testing;

// Mock classes for testing
class MockADC : public WindVane::IADC {
public:
    MOCK_METHOD(float, ReadNormalized, (), (const, override));
    MOCK_METHOD(int, ReadRaw, (), (const, override));
    MOCK_METHOD(bool, IsAvailable, (), (const, override));
    MOCK_METHOD(int, GetResolutionBits, (), (const, override));
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

        WindVane::WindVane::Config config{
            *mockADC,
            mockStorage.get(),
            *mockUserIO,
            *mockDiagnostics
        };

        windVane = std::make_unique<WindVane::WindVane>(config);
    }
};

TEST_F(WindVaneCoreTest, Constructor_ValidConfig_CreatesInstance) {
    EXPECT_NE(windVane, nullptr);
}

TEST_F(WindVaneCoreTest, GetDirection_NoCalibration_ReturnsZero) {
    EXPECT_CALL(*mockStorage, HasCalibration())
        .WillOnce(Return(false));

    float direction = windVane->GetDirection();
    EXPECT_EQ(direction, 0.0f);
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
    EXPECT_CALL(*mockADC, ReadNormalized())
        .WillOnce(Return(0.5f)); // 180 degrees

    float direction = windVane->GetDirection();
    EXPECT_NEAR(direction, 225.0f, 0.1f); // 180 + 45 offset
}

TEST_F(WindVaneCoreTest, GetRawReading_ReturnsADCRawValue) {
    EXPECT_CALL(*mockADC, ReadNormalized())
        .WillOnce(Return(0.75f));

    float rawValue = windVane->GetRawReading();
    EXPECT_EQ(rawValue, 0.75f);
}

TEST_F(WindVaneCoreTest, IsCalibrated_NoCalibration_ReturnsFalse) {
    EXPECT_CALL(*mockStorage, HasCalibration())
        .WillOnce(Return(false));

    bool isCalibrated = windVane->IsCalibrated();
    EXPECT_FALSE(isCalibrated);
}

TEST_F(WindVaneCoreTest, IsCalibrated_WithCalibration_ReturnsTrue) {
    EXPECT_CALL(*mockStorage, HasCalibration())
        .WillOnce(Return(true));

    bool isCalibrated = windVane->IsCalibrated();
    EXPECT_TRUE(isCalibrated);
}

TEST_F(WindVaneCoreTest, ClearCalibration_CallsStorageClear) {
    EXPECT_CALL(*mockStorage, ClearCalibration())
        .WillOnce(Return(true));

    bool result = windVane->ClearCalibration();
    EXPECT_TRUE(result);
}

TEST_F(WindVaneCoreTest, StartCalibration_ValidMethod_ReturnsSuccess) {
    EXPECT_CALL(*mockDiagnostics, LogInfo(_))
        .Times(AtLeast(1));

    auto result = windVane->StartCalibration();
    EXPECT_TRUE(result.success);
}

TEST_F(WindVaneCoreTest, GetLastCalibrationTimestamp_ReturnsTimestamp) {
    uint64_t timestamp = windVane->GetLastCalibrationTimestamp();
    EXPECT_GE(timestamp, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}