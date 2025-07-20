#pragma once
#include <Diagnostics/IDiagnostics.h>
#include <UI/IIO.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include "MenuLogic.h"
#include "MenuPresenter.h"
#include "MenuDisplayView.h"
#include "MenuDisplayController.h"
#include "MenuState.h"
#include "MenuTypes.h"
#include <Platform/IPlatform.h>
#include <Storage/Settings/SettingsManager.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <optional>

/** Dependencies required by the menu. All references are borrowed and must
 * outlive the menu instance. */
struct WindVaneMenuConfig {
  WindVane& vane;
  IUserIO& io;
  IDiagnostics& diag;
  std::optional<std::reference_wrapper<IBufferedDiagnostics>> bufferedDiag;
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
  std::optional<std::reference_wrapper<IBufferedDiagnostics>> _buffered;
  IOutput& _out;
  ICalibrationStorage& _storage;
  SettingsManager& _settingsMgr;
  IPlatform& _platform;

  MenuLogic _logic;
  MenuPresenter _presenter;
  MenuState _state;
  MenuDisplayView _view;
  MenuDisplayController _display;
  enum class State {
    Main,
    LiveDisplay,
    Calibrate,
    Diagnostics,
    Settings,
    Help
  };
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
