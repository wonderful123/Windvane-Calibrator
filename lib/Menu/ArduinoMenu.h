#pragma once
#include <WindVane.h>
#include <IO/IIOHandler.h>
#include <Diagnostics/IDiagnostics.h>
#include <Diagnostics/BufferedDiagnostics.h>
#include <Storage/ICalibrationStorage.h>
#include <Settings/ISettingsStorage.h>
#include <Settings/SettingsData.h>
#include <string>

class ArduinoMenu {
public:
    ArduinoMenu(WindVane* vane, IIOHandler* io, IDiagnostics* diag,
                ICalibrationStorage* storage = nullptr,
                ISettingsStorage* settingsStorage = nullptr,
                SettingsData* settings = nullptr);
    void begin();
    void update();
private:
    enum class State { Main, LiveDisplay, Calibrate, Diagnostics, Settings, Help };

    WindVane* _vane;
    IIOHandler* _io;
    IDiagnostics* _diag;
    BufferedDiagnostics* _buffered{nullptr};
    ICalibrationStorage* _storage{nullptr};
    ISettingsStorage* _settingsStorage{nullptr};
    SettingsData* _settings{nullptr};
    std::string _statusMsg;
    StatusLevel _statusLevel{StatusLevel::Normal};
    unsigned long _msgExpiry{0};
    State _state;
    unsigned long _lastActivity;
    unsigned long _lastCalibration;

    void showMainMenu();
    void showStatusLine();
    void handleMainInput(char c);
    void updateLiveDisplay();
    void runCalibration();
    void showDiagnostics();
    void settingsMenu();
    void showHelp();
    void selfTest();
    enum class StatusLevel { Normal, Warning, Error };
    void setStatusMessage(const char* msg, StatusLevel lvl = StatusLevel::Normal,
                          unsigned long ms = 3000);
    void clearScreen();
    float readFloat();
    int readInt();
};
