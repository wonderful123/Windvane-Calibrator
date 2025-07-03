#pragma once
#include <Diagnostics/IDiagnostics.h>
#include <UI/IIO.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include "WindVaneMenuLogic.h"
#include "WindVaneMenuPresenter.h"
#include "WindVaneMenuDisplayController.h"
#include "WindVaneMenuTypes.h"
#include <Platform/IPlatform.h>
#include <Storage/Settings/SettingsManager.h>
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
  IPlatform& platform;
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
  IPlatform& _platform;

  WindVaneMenuLogic _logic;
  WindVaneMenuPresenter _presenter;
  WindVaneMenuDisplayController _display;
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

  void showMainMenu() const;
  void handleMainInput(char c);

  MenuResult runCalibration();
  void handleDisplaySelection();
  void handleCalibrateSelection();
  void handleDiagnosticsSelection();
  void handleSettingsSelectionMenu();
  void handleHelpSelection();
  void handleUnknownSelection();
  void showHelp() const;
  void clearScreen() const;
  void pushState(State s);
  void popState();
  State currentState() const;
  void initMainHandlers();
};
