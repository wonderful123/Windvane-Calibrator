#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindVane/Calibration/CalibrationManager.h"
#include "WindVane/Calibration/ICalibrationStrategy.h"
#include "WindVane/Interfaces/IUserIO.h"
#include "WindVane/Interfaces/IDiagnostics.h"

using namespace testing;

// Mock calibration strategy
class MockCalibrationStrategy : public WindVane::ICalibrationStrategy {
public:
    MOCK_METHOD(bool, Start, (), (override));
    MOCK_METHOD(bool, Update, (), (override));
    MOCK_METHOD(bool, IsComplete, (), (const, override));
    MOCK_METHOD(WindVane::CalibrationResult, GetResult, (), (const, override));
    MOCK_METHOD(float, GetProgress, (), (const, override));
    MOCK_METHOD(void, Cancel, (), (override));
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

class CalibrationManagerTest : public Test {
protected:
    std::shared_ptr<MockCalibrationStrategy> mockStrategy;
    std::shared_ptr<MockUserIO> mockUserIO;
    std::shared_ptr<MockDiagnostics> mockDiagnostics;
    std::unique_ptr<WindVane::CalibrationManager> calibrationManager;

    void SetUp() override {
        mockStrategy = std::make_shared<MockCalibrationStrategy>();
        mockUserIO = std::make_shared<MockUserIO>();
        mockDiagnostics = std::make_shared<MockDiagnostics>();

        calibrationManager = std::make_unique<WindVane::CalibrationManager>(
            mockUserIO, mockDiagnostics);
    }
};

TEST_F(CalibrationManagerTest, Constructor_CreatesInstance) {
    EXPECT_NE(calibrationManager, nullptr);
}

TEST_F(CalibrationManagerTest, StartCalibration_ValidStrategy_ReturnsSuccess) {
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockDiagnostics, LogInfo(_))
        .Times(AtLeast(1));

    auto result = calibrationManager->StartCalibration(mockStrategy);
    EXPECT_TRUE(result.success);
}

TEST_F(CalibrationManagerTest, StartCalibration_StrategyFails_ReturnsFailure) {
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(false));
    EXPECT_CALL(*mockDiagnostics, LogError(_))
        .Times(AtLeast(1));

    auto result = calibrationManager->StartCalibration(mockStrategy);
    EXPECT_FALSE(result.success);
}

TEST_F(CalibrationManagerTest, Update_CalibrationInProgress_UpdatesStrategy) {
    // Start calibration
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    calibrationManager->StartCalibration(mockStrategy);

    // Update calibration
    EXPECT_CALL(*mockStrategy, Update())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStrategy, IsComplete())
        .WillOnce(Return(false));

    bool result = calibrationManager->Update();
    EXPECT_TRUE(result);
}

TEST_F(CalibrationManagerTest, Update_CalibrationComplete_ReturnsFalse) {
    // Start calibration
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    calibrationManager->StartCalibration(mockStrategy);

    // Complete calibration
    EXPECT_CALL(*mockStrategy, Update())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStrategy, IsComplete())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStrategy, GetResult())
        .WillOnce(Return(WindVane::CalibrationResult{true, "Success"}));

    bool result = calibrationManager->Update();
    EXPECT_FALSE(result);
}

TEST_F(CalibrationManagerTest, IsCalibrating_NoCalibration_ReturnsFalse) {
    bool isCalibrating = calibrationManager->IsCalibrating();
    EXPECT_FALSE(isCalibrating);
}

TEST_F(CalibrationManagerTest, IsCalibrating_CalibrationActive_ReturnsTrue) {
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    calibrationManager->StartCalibration(mockStrategy);

    bool isCalibrating = calibrationManager->IsCalibrating();
    EXPECT_TRUE(isCalibrating);
}

TEST_F(CalibrationManagerTest, GetProgress_NoCalibration_ReturnsZero) {
    float progress = calibrationManager->GetProgress();
    EXPECT_EQ(progress, 0.0f);
}

TEST_F(CalibrationManagerTest, GetProgress_CalibrationActive_ReturnsStrategyProgress) {
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    calibrationManager->StartCalibration(mockStrategy);

    EXPECT_CALL(*mockStrategy, GetProgress())
        .WillOnce(Return(0.5f));

    float progress = calibrationManager->GetProgress();
    EXPECT_EQ(progress, 0.5f);
}

TEST_F(CalibrationManagerTest, CancelCalibration_ActiveCalibration_CancelsStrategy) {
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    calibrationManager->StartCalibration(mockStrategy);

    EXPECT_CALL(*mockStrategy, Cancel())
        .Times(1);

    calibrationManager->CancelCalibration();
    EXPECT_FALSE(calibrationManager->IsCalibrating());
}

TEST_F(CalibrationManagerTest, GetCalibrationResult_NoCalibration_ReturnsEmptyResult) {
    auto result = calibrationManager->GetCalibrationResult();
    EXPECT_FALSE(result.success);
}

TEST_F(CalibrationManagerTest, GetCalibrationResult_CompletedCalibration_ReturnsResult) {
    WindVane::CalibrationResult expectedResult{true, "Calibration successful"};
    
    EXPECT_CALL(*mockStrategy, Start())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStrategy, Update())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStrategy, IsComplete())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockStrategy, GetResult())
        .WillOnce(Return(expectedResult));

    calibrationManager->StartCalibration(mockStrategy);
    calibrationManager->Update();

    auto result = calibrationManager->GetCalibrationResult();
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.message, expectedResult.message);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}