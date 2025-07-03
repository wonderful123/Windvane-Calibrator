#pragma once
#include <WindVane.h>
#include <IO/IIOHandler.h>
#include <Diagnostics/IDiagnostics.h>
#include <Diagnostics/BufferedDiagnostics.h>
#include <Storage/ICalibrationStorage.h>
#include <Settings/ISettingsStorage.h>
#include <Settings/SettingsData.h>
#include <string>

struct ArduinoMenuConfig {
    WindVane* vane{};
    IIOHandler* io{};
    IDiagnostics* diag{};
    ICalibrationStorage* storage{nullptr};
    ISettingsStorage* settingsStorage{nullptr};
    SettingsData* settings{nullptr};
};

class ArduinoMenu {
public:
    explicit ArduinoMenu(const ArduinoMenuConfig& cfg);
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
    const char* statusText(CalibrationManager::CalibrationStatus st) const;
    void renderStatusLineArduino(float dir, const char* statusStr, unsigned long ago);
    void renderStatusLineHost(float dir, const char* statusStr, unsigned long ago);
    void clearExpiredMessage();
    void handleMainInput(char c);
    void handleDisplaySelection();
    void handleCalibrateSelection();
    void handleDiagnosticsSelection();
    void handleSettingsSelectionMenu();
    void handleHelpSelection();
    void handleUnknownSelection();
    void updateLiveDisplay();
    void runCalibration();
    void showDiagnostics();
    void settingsMenu();
    void showHelp();
    char readCharBlocking();
    void renderDiagnosticsScreen(size_t index);
    void handleDiagnosticsAction(char c, size_t &index, bool &exit);
    void showSettingsSummary();
    bool handleSettingsSelection(char c);
    void selfTest();
    enum class StatusLevel { Normal, Warning, Error };
    void setStatusMessage(const char* msg, StatusLevel lvl = StatusLevel::Normal,
                          unsigned long ms = 3000);
    void clearScreen();
    float readFloat();
    int readInt();
};
