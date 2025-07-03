#pragma once
#include <Diagnostics/IBufferedDiagnostics.h>
#include <Diagnostics/IDiagnostics.h>
#include <IO/IIOHandler.h>
#include <IO/INumericReader.h>  // <-- ADD THIS INCLUDE
#include <IO/IOutput.h>
#include <Settings/ISettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include "WindVaneMenuLogic.h"
#include "WindVaneMenuPresenter.h"
#include "WindVaneMenuTypes.h"

#include <string>

struct WindVaneMenuConfig {
  WindVane* vane{};
  IIOHandler* io{};
  IDiagnostics* diag{};
  IBufferedDiagnostics* bufferedDiag{};
  IOutput* out{};
  ICalibrationStorage* storage{};
  ISettingsStorage* settingsStorage{};
  SettingsData* settings{};
  INumericReader* numeric{};  // <-- ADD TO CONFIG, pass to SettingsMenu
};

class WindVaneMenu {
 public:
  explicit WindVaneMenu(const WindVaneMenuConfig& cfg);
  void begin();
  void update();

 private:
  WindVane* _vane;
  IIOHandler* _io;
  IDiagnostics* _diag;
  IBufferedDiagnostics* _buffered;
  IOutput* _out;
  ICalibrationStorage* _storage;
  ISettingsStorage* _settingsStorage;
  SettingsData* _settings;
  INumericReader* _numeric;  // <-- DECLARE _numeric

  WindVaneMenuLogic _logic;
  WindVaneMenuPresenter _presenter;
  enum class State {
    Main,
    LiveDisplay,
    Calibrate,
    Diagnostics,
    Settings,
    Help
  };
  State _state;
  unsigned long _lastActivity;
  unsigned long _lastCalibration;

  std::string _statusMsg;
  MenuStatusLevel _statusLevel{MenuStatusLevel::Normal};
  unsigned long _msgExpiry{0};

  void showStatusLine();
  void showMainMenu();
  void handleMainInput(char c);
  void updateLiveDisplay();
  void clearExpiredMessage();
  void runCalibration();
  void handleDisplaySelection();
  void handleCalibrateSelection();
  void handleDiagnosticsSelection();
  void handleSettingsSelectionMenu();
  void handleHelpSelection();
  void handleUnknownSelection();
  void showHelp();
  void clearScreen();
  void setStatusMessage(const char* msg, MenuStatusLevel lvl = MenuStatusLevel::Normal,
                        unsigned long ms = 3000);
};
