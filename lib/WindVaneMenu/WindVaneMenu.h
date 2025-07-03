#pragma once
#include <Diagnostics/IDiagnostics.h>
#include <IO/IIO.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include "WindVaneMenuLogic.h"
#include "WindVaneMenuPresenter.h"
#include "WindVaneMenuTypes.h"
#include <Settings/SettingsManager.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

/** Dependencies required by the menu. All references are borrowed and must
 * outlive the menu instance. */
struct WindVaneMenuConfig {
  WindVane& vane;
  IUserIO& io;
  IDiagnostics& diag;
  IBufferedDiagnostics* bufferedDiag;
  IOutput& out;
  ICalibrationStorage& storage;
  SettingsManager& settingsMgr;
};

class WindVaneMenu {
 public:
  explicit WindVaneMenu(const WindVaneMenuConfig& cfg);
  void begin();
  void update();

 private:
  WindVane& _vane;
  IUserIO& _io;
  IDiagnostics& _diag;
  IBufferedDiagnostics* _buffered;
  IOutput& _out;
  ICalibrationStorage& _storage;
  SettingsManager& _settingsMgr;

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
  std::vector<State> _stateStack;
  std::unordered_map<char, std::function<void()>> _mainHandlers;
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
  void pushState(State s);
  void popState();
  State currentState() const;
  void initMainHandlers();
};
