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
      _numeric(cfg.numeric),
      _logic(),
      _presenter(&cfg.out),
      _state(State::Main),
      _lastActivity(0),
      _lastCalibration(0) {}

void WindVaneMenu::begin() {
  showMainMenu();
  _lastActivity = platformMillis();
  _lastCalibration = _vane.lastCalibrationTimestamp();
}

void WindVaneMenu::update() {
  if (_io.hasInput()) {
    char c = _io.readInput();
    _lastActivity = platformMillis();
    switch (_state) {
      case State::Main:
        handleMainInput(c);
        break;
      case State::LiveDisplay:
        _state = State::Main;
        showMainMenu();
        break;
      default:
        break;
    }
  }
  switch (_state) {
    case State::LiveDisplay:
      updateLiveDisplay();
      break;
    default:
      break;
  }
  if (platformMillis() - _lastActivity > 30000 && _state != State::Main) {
    _state = State::Main;
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
  switch (c) {
    case 'D':
    case 'd':
      handleDisplaySelection();
      break;
    case 'C':
    case 'c':
      handleCalibrateSelection();
      break;
    case 'G':
    case 'g':
      handleDiagnosticsSelection();
      break;
    case 'S':
    case 's':
      handleSettingsSelectionMenu();
      break;
    case 'H':
    case 'h':
      handleHelpSelection();
      break;
    default:
      handleUnknownSelection();
      break;
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
    _state = State::Main;
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
  _state = State::LiveDisplay;
  if (_vane.calibrationStatus() !=
      CalibrationManager::CalibrationStatus::Completed)
    setStatusMessage("Warning: uncalibrated", MenuStatusLevel::Warning);
  _out.writeln("Live direction - press any key to return");
}

void WindVaneMenu::handleCalibrateSelection() {
  _state = State::Calibrate;
  runCalibration();
  _state = State::Main;
  showMainMenu();
}

void WindVaneMenu::handleDiagnosticsSelection() {
  _state = State::Diagnostics;
  DiagnosticsMenu menu(_vane, _io, _buffered, _diag, _out);
  menu.show(_lastCalibration);
  _state = State::Main;
  showMainMenu();
}

void WindVaneMenu::handleSettingsSelectionMenu() {
  _state = State::Settings;
  SettingsMenu menu(&_vane, &_io, &_numeric, &_storage, &_settingsMgr, &_out);
  _state = State::Main;
  showMainMenu();
}

void WindVaneMenu::handleHelpSelection() {
  _state = State::Help;
  showHelp();
  _state = State::Main;
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
