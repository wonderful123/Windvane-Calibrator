#pragma once
#include <WindVane.h>
#include <IO/IIOHandler.h>
#include <Diagnostics/IDiagnostics.h>

class ArduinoMenu {
public:
    ArduinoMenu(WindVane* vane, IIOHandler* io, IDiagnostics* diag);
    void begin();
    void update();
private:
    enum class State { Main, LiveDisplay, Calibrate, Diagnostics, Settings, Help };

    WindVane* _vane;
    IIOHandler* _io;
    IDiagnostics* _diag;
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
};
