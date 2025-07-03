#include "WindVaneMenu.h"

#include "DiagnosticsMenu.h"
#include "SettingsMenu.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <chrono>
#include <cstdio>
#include <limits>
static unsigned long millis() {
  using namespace std::chrono;
  static auto start = steady_clock::now();
  return duration_cast<milliseconds>(steady_clock::now() - start).count();
}
#endif

static const char* compassPoint(float deg) {
  static const char* pts[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
  int idx = static_cast<int>((deg + 22.5f) / 45.0f) & 7;
  return pts[idx];
}

WindVaneMenu::WindVaneMenu(const WindVaneMenuConfig& cfg)
    : _vane(cfg.vane),
      _io(cfg.io),
      _diag(cfg.diag),
      _buffered(cfg.bufferedDiag),
      _out(cfg.out),
      _storage(cfg.storage),
      _settingsStorage(cfg.settingsStorage),
      _settings(cfg.settings),
      _numeric(cfg.numeric),
      _logic(),
      _presenter(&cfg.out),
      _state(State::Main),
      _lastActivity(0),
      _lastCalibration(0) {}

void WindVaneMenu::begin() {
  showMainMenu();
  _lastActivity = millis();
  _lastCalibration = _vane.lastCalibrationTimestamp();
}

void WindVaneMenu::update() {
  if (_io.hasInput()) {
    char c = _io.readInput();
    _lastActivity = millis();
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
  if (millis() - _lastActivity > 30000 && _state != State::Main) {
    _state = State::Main;
    showMainMenu();
  }
  showStatusLine();
}

void WindVaneMenu::showStatusLine() {
  WindVaneStatus st = _logic.queryStatus(_vane, _lastCalibration);
  const char* statusStr = _logic.statusText(st.calibrationStatus);
#ifdef ARDUINO
  _presenter.renderStatusLineArduino(st, statusStr, _statusMsg, _statusLevel);
#else
  _presenter.renderStatusLineHost(st, statusStr, _statusMsg, _statusLevel);
#endif
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
  if (millis() - last > 1000) {
    last = millis();
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
  if (!_statusMsg.empty() && millis() >= _msgExpiry) {
    _statusMsg.clear();
    _statusLevel = MenuStatusLevel::Normal;
  }
}

void WindVaneMenu::runCalibration() {
  if (_io.yesNoPrompt("Start calibration? (Y/N)")) {
    _vane.runCalibration();
    _lastCalibration = millis();
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
  SettingsMenu menu(&_vane, &_io, &_numeric, &_storage, &_settingsStorage, &_settings, &_out);
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
  _msgExpiry = millis() + ms;
}
