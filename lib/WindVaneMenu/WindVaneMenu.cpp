#include "WindVaneMenu.h"

#include "DiagnosticsMenu.h"
#include "SettingsMenu.h"
#include "WindVaneCompass.h"
#include <Platform/Platform.h>

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
      _logic(),
        _presenter(&cfg.out),
        _stateStack(),
        _mainHandlers(),
        _lastActivity(0),
        _lastCalibration(0) {
  pushState(State::Main);
  initMainHandlers();
}

void WindVaneMenu::begin() {
  _stateStack.clear();
  pushState(State::Main);
  showMainMenu();
  _lastActivity = platformMillis();
  _lastCalibration = _vane.lastCalibrationTimestamp();
}

void WindVaneMenu::update() {
  if (_io.hasInput()) {
    char c = _io.readInput();
    _lastActivity = platformMillis();
    if (currentState() == State::Main) {
      handleMainInput(c);
    } else if (currentState() == State::LiveDisplay) {
      popState();
      showMainMenu();
    }
  }
  if (currentState() == State::LiveDisplay) {
    updateLiveDisplay();
  }
  if (platformMillis() - _lastActivity > 30000 && currentState() != State::Main) {
    while (currentState() != State::Main) popState();
    showMainMenu();
  }
  showStatusLine();
}

void WindVaneMenu::showStatusLine() {
  WindVaneStatus st = _logic.queryStatus(_vane, _lastCalibration);
  const char* statusStr = _logic.statusText(st.calibrationStatus);
  PLATFORM_RENDER_STATUSLINE(_presenter, st, statusStr, _statusMsg, _statusLevel);
  clearExpiredMessage();
}

void WindVaneMenu::showMainMenu() {
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

void WindVaneMenu::updateLiveDisplay() {
  static unsigned long last = 0;
  if (platformMillis() - last > 1000) {
    last = platformMillis();
    float d = _vane.direction();
    char buf[64];
    snprintf(buf, sizeof(buf), "\rDir: %.1f\xC2\xB0 (%s)   \r", d,
             compassPoint(d));
    _out.write(buf);
  }
  if (_io.hasInput()) {
    _io.readInput();
    popState();
    showMainMenu();
  }
}


void WindVaneMenu::clearExpiredMessage() {
  if (!_statusMsg.empty() && platformMillis() >= _msgExpiry) {
    _statusMsg.clear();
    _statusLevel = MenuStatusLevel::Normal;
  }
}

void WindVaneMenu::runCalibration() {
  if (_io.yesNoPrompt("Start calibration? (Y/N)")) {
    _vane.runCalibration();
    _lastCalibration = platformMillis();
    _diag.info("Calibration completed");
    setStatusMessage("Calibration complete", MenuStatusLevel::Normal);
  }
}

void WindVaneMenu::handleDisplaySelection() {
  pushState(State::LiveDisplay);
  if (_vane.calibrationStatus() !=
      CalibrationManager::CalibrationStatus::Completed)
    setStatusMessage("Warning: uncalibrated", MenuStatusLevel::Warning);
  _out.writeln("Live direction - press any key to return");
}

void WindVaneMenu::handleCalibrateSelection() {
  pushState(State::Calibrate);
  runCalibration();
  popState();
  showMainMenu();
}

void WindVaneMenu::handleDiagnosticsSelection() {
  pushState(State::Diagnostics);
  DiagnosticsMenu menu(_vane, _io, _buffered, _diag, _out);
  menu.show(_lastCalibration);
  popState();
  showMainMenu();
}

void WindVaneMenu::handleSettingsSelectionMenu() {
  pushState(State::Settings);
  SettingsMenu menu(&_vane, &_io, &_storage, &_settingsMgr, &_out);
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
  setStatusMessage("Unknown option. Press [H] for help.", MenuStatusLevel::Error);
}

void WindVaneMenu::showHelp() {
  _out.writeln("--- Help ---");
  _out.writeln("D: Live wind direction display");
  _out.writeln("C: Start calibration routine");
  _out.writeln("G: View diagnostics log");
  _out.writeln("S: Settings and maintenance");
  _out.writeln("H: Show this help text");
}

void WindVaneMenu::clearScreen() { _out.clear(); }

void WindVaneMenu::setStatusMessage(const char* msg, MenuStatusLevel lvl,
                                    unsigned long ms) {
  _statusMsg = msg;
  _statusLevel = lvl;
  _msgExpiry = platformMillis() + ms;
}

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
