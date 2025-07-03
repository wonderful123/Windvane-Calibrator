#include "WindVaneMenu.h"

#include "DiagnosticsMenu.h"
#include "SettingsMenu.h"
#include "WindVaneCompass.h"
#include "MenuDisplayController.h"
#include <Calibration/CalibrationResult.h>
#include <Platform/Platform.h>
#include <Platform/IPlatform.h>

#ifndef ARDUINO
#include <cstdio>
#include <limits>
#endif


WindVaneMenu::WindVaneMenu(const WindVaneMenuConfig& cfg)
    : _vane(cfg.vane),
      _io(cfg.io),
      _diag(cfg.diag),
      _buffered(cfg.bufferedDiag),
      _out(cfg.out),
      _storage(cfg.storage),
      _settingsMgr(cfg.settingsMgr),
      _platform(cfg.platform),
      _logic(),
      _presenter(&cfg.out),
      _display(cfg.platform, cfg.io, cfg.out, _presenter, _logic),
      _stateStack(),
      _mainHandlers() {
  pushState(State::Main);
  initMainHandlers();
}

void WindVaneMenu::begin() {
  _stateStack.clear();
  pushState(State::Main);
  showMainMenu();
  _display.begin(_vane);
}

void WindVaneMenu::update() {
  if (_io.hasInput()) {
    char c = _io.readInput();
    _display.onInput();
    if (currentState() == State::Main) {
      handleMainInput(c);
    } else if (currentState() == State::LiveDisplay) {
      popState();
      showMainMenu();
    }
  }
  if (currentState() == State::LiveDisplay) {
    if (_display.updateLiveDisplay(_vane)) {
      popState();
      showMainMenu();
    }
  }
  if (_display.checkTimeout() && currentState() != State::Main) {
    while (currentState() != State::Main) popState();
    showMainMenu();
  }
  _display.showStatusLine(_vane);
}

void WindVaneMenu::showMainMenu() const {
  clearScreen();
  _out.writeln("");
  _out.writeln("=== Wind Vane Menu ===");
  _out.writeln("[D] Display direction ");
  _out.writeln("[C] Calibrate        ");
  _out.writeln("[G] Diagnostics      ");
  _out.writeln("[S] Settings         ");
  _out.writeln("[H] Help             ");
#ifndef ARDUINO
  _out.writeln("Choose option: ");
#endif
}

void WindVaneMenu::handleMainInput(char c) {
  if (c == '\n' || c == '\r') {
    showMainMenu();
    return;
  }
  auto it = _mainHandlers.find(c);
  if (it != _mainHandlers.end()) {
    it->second();
  } else {
    handleUnknownSelection();
  }
}


MenuResult WindVaneMenu::runCalibration() {
  MenuResult out;
  if (!_io.yesNoPrompt("Start calibration? (Y/N)")) {
    out.message = "Calibration cancelled";
    return out;
  }
  CalibrationResult res = _vane.runCalibration();
  if (res.success) {
    _display.recordCalibration();
    out.success = true;
    out.message = "Calibration complete";
  } else {
    out.message = res.error;
  }
  return out;
}

void WindVaneMenu::handleDisplaySelection() {
  pushState(State::LiveDisplay);
  if (_vane.calibrationStatus() !=
      CalibrationManager::CalibrationStatus::Completed)
    _display.setStatusMessage("Warning: uncalibrated", MenuStatusLevel::Warning);
  _out.writeln("Live direction - press any key to return");
}

void WindVaneMenu::handleCalibrateSelection() {
  pushState(State::Calibrate);
  MenuResult r = runCalibration();
  if (r.success) {
    _diag.info(r.message.c_str());
    _display.setStatusMessage(r.message.c_str(), MenuStatusLevel::Normal);
  } else if (!r.message.empty()) {
    _diag.warn(r.message.c_str());
    _display.setStatusMessage(r.message.c_str(), MenuStatusLevel::Error);
  }
  popState();
  showMainMenu();
}

void WindVaneMenu::handleDiagnosticsSelection() {
  pushState(State::Diagnostics);
  DiagnosticsMenu menu(_vane, _io, _buffered, _diag, _out, _platform);
  menu.show(_display.lastCalibration());
  popState();
  showMainMenu();
}

void WindVaneMenu::handleSettingsSelectionMenu() {
  pushState(State::Settings);
  SettingsMenu menu(_vane, _io, _storage, _settingsMgr, _out);
  popState();
  showMainMenu();
}

void WindVaneMenu::handleHelpSelection() {
  pushState(State::Help);
  showHelp();
  popState();
  showMainMenu();
}

void WindVaneMenu::handleUnknownSelection() {
  _display.setStatusMessage("Unknown option. Press [H] for help.",
                            MenuStatusLevel::Error);
}

void WindVaneMenu::showHelp() const {
  _out.writeln("--- Help ---");
  _out.writeln("D: Live wind direction display");
  _out.writeln("C: Start calibration routine");
  _out.writeln("G: View diagnostics log");
  _out.writeln("S: Settings and maintenance");
  _out.writeln("H: Show this help text");
}

void WindVaneMenu::clearScreen() const { _out.clear(); }

void WindVaneMenu::pushState(State s) { _stateStack.push_back(s); }

void WindVaneMenu::popState() {
  if (!_stateStack.empty()) _stateStack.pop_back();
}

WindVaneMenu::State WindVaneMenu::currentState() const {
  return _stateStack.empty() ? State::Main : _stateStack.back();
}

void WindVaneMenu::initMainHandlers() {
  _mainHandlers.clear();
  _mainHandlers['D'] = [this] { handleDisplaySelection(); };
  _mainHandlers['d'] = [this] { handleDisplaySelection(); };
  _mainHandlers['C'] = [this] { handleCalibrateSelection(); };
  _mainHandlers['c'] = [this] { handleCalibrateSelection(); };
  _mainHandlers['G'] = [this] { handleDiagnosticsSelection(); };
  _mainHandlers['g'] = [this] { handleDiagnosticsSelection(); };
  _mainHandlers['S'] = [this] { handleSettingsSelectionMenu(); };
  _mainHandlers['s'] = [this] { handleSettingsSelectionMenu(); };
  _mainHandlers['H'] = [this] { handleHelpSelection(); };
  _mainHandlers['h'] = [this] { handleHelpSelection(); };
}
