#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindVane/Menu/WindVaneMenu.h"
#include "WindVane/Menu/MenuController.h"
#include "WindVane/Menu/MenuPresenter.h"
#include "WindVane/Interfaces/IUserIO.h"
#include "WindVane/Interfaces/IDiagnostics.h"

using namespace testing;

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

class MockWindVane {
public:
    MOCK_METHOD(float, GetDirection, (), (const));
    MOCK_METHOD(float, GetRawReading, (), (const));
    MOCK_METHOD(bool, IsCalibrated, (), (const));
    MOCK_METHOD(WindVane::CalibrationResult, StartCalibration, (), ());
    MOCK_METHOD(bool, ClearCalibration, (), ());
};

class MenuSystemTest : public Test {
protected:
    std::shared_ptr<MockUserIO> mockUserIO;
    std::shared_ptr<MockDiagnostics> mockDiagnostics;
    std::shared_ptr<MockWindVane> mockWindVane;
    std::unique_ptr<WindVane::WindVaneMenu> menu;

    void SetUp() override {
        mockUserIO = std::make_shared<MockUserIO>();
        mockDiagnostics = std::make_shared<MockDiagnostics>();
        mockWindVane = std::make_shared<MockWindVane>();

        menu = std::make_unique<WindVane::WindVaneMenu>(
            mockUserIO, mockDiagnostics, mockWindVane);
    }
};

TEST_F(MenuSystemTest, Constructor_CreatesMenuInstance) {
    EXPECT_NE(menu, nullptr);
}

TEST_F(MenuSystemTest, ShowMainMenu_DisplaysMenuOptions) {
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));
    EXPECT_CALL(*mockUserIO, Println(_))
        .Times(AtLeast(1));

    menu->ShowMainMenu();
}

TEST_F(MenuSystemTest, HandleInput_ValidOption_ExecutesAction) {
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('1')); // Direction display option
    EXPECT_CALL(*mockWindVane, GetDirection())
        .WillOnce(Return(180.0f));
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->HandleInput();
}

TEST_F(MenuSystemTest, HandleInput_CalibrationOption_StartsCalibration) {
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('2')); // Calibration option
    EXPECT_CALL(*mockWindVane, StartCalibration())
        .WillOnce(Return(WindVane::CalibrationResult{true, "Success"}));
    EXPECT_CALL(*mockUserIO, Println(_))
        .Times(AtLeast(1));

    menu->HandleInput();
}

TEST_F(MenuSystemTest, HandleInput_DiagnosticsOption_ShowsDiagnostics) {
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('3')); // Diagnostics option
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->HandleInput();
}

TEST_F(MenuSystemTest, HandleInput_SettingsOption_ShowsSettings) {
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('4')); // Settings option
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->HandleInput();
}

TEST_F(MenuSystemTest, HandleInput_HelpOption_ShowsHelp) {
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('5')); // Help option
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->HandleInput();
}

TEST_F(MenuSystemTest, HandleInput_QuitOption_ExitsMenu) {
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('q')); // Quit option
    EXPECT_CALL(*mockUserIO, Println(_))
        .Times(AtLeast(1));

    menu->HandleInput();
}

TEST_F(MenuSystemTest, ShowDirectionDisplay_ShowsCurrentDirection) {
    EXPECT_CALL(*mockWindVane, GetDirection())
        .WillOnce(Return(270.0f));
    EXPECT_CALL(*mockWindVane, IsCalibrated())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->ShowDirectionDisplay();
}

TEST_F(MenuSystemTest, ShowDirectionDisplay_NotCalibrated_ShowsWarning) {
    EXPECT_CALL(*mockWindVane, IsCalibrated())
        .WillOnce(Return(false));
    EXPECT_CALL(*mockUserIO, Println(_))
        .Times(AtLeast(1));

    menu->ShowDirectionDisplay();
}

TEST_F(MenuSystemTest, ShowCalibrationMenu_StartsCalibration) {
    EXPECT_CALL(*mockWindVane, StartCalibration())
        .WillOnce(Return(WindVane::CalibrationResult{true, "Calibration started"}));
    EXPECT_CALL(*mockUserIO, Println(_))
        .Times(AtLeast(1));

    menu->ShowCalibrationMenu();
}

TEST_F(MenuSystemTest, ShowDiagnosticsMenu_ShowsSystemInfo) {
    EXPECT_CALL(*mockWindVane, GetRawReading())
        .WillOnce(Return(0.75f));
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->ShowDiagnosticsMenu();
}

TEST_F(MenuSystemTest, ShowSettingsMenu_ShowsSettingsOptions) {
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->ShowSettingsMenu();
}

TEST_F(MenuSystemTest, ShowHelpMenu_ShowsHelpInformation) {
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));

    menu->ShowHelpMenu();
}

TEST_F(MenuSystemTest, RunMenuLoop_HandlesMultipleInputs) {
    // Simulate menu loop with multiple inputs
    EXPECT_CALL(*mockUserIO, ReadChar())
        .WillOnce(Return('1'))  // Direction display
        .WillOnce(Return('2'))  // Calibration
        .WillOnce(Return('q')); // Quit
    EXPECT_CALL(*mockWindVane, GetDirection())
        .WillOnce(Return(90.0f));
    EXPECT_CALL(*mockWindVane, IsCalibrated())
        .WillOnce(Return(true));
    EXPECT_CALL(*mockWindVane, StartCalibration())
        .WillOnce(Return(WindVane::CalibrationResult{true, "Success"}));
    EXPECT_CALL(*mockUserIO, Print(_))
        .Times(AtLeast(1));
    EXPECT_CALL(*mockUserIO, Println(_))
        .Times(AtLeast(1));

    // Run a few iterations of the menu loop
    for (int i = 0; i < 3; ++i) {
        menu->HandleInput();
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}