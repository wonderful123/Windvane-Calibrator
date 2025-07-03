#include "ArduinoMenu.h"
#include "DiagnosticsMenu.h"
#include "SettingsMenu.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <chrono>
#include <iostream>
#include <limits>
#include <cstdio>
using std::cout; using std::endl;
static unsigned long millis() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - start).count();
}
#endif

static const char* compassPoint(float deg) {
    static const char* pts[] = {"N","NE","E","SE","S","SW","W","NW"};
    int idx = static_cast<int>((deg + 22.5f)/45.0f) & 7;
    return pts[idx];
}

ArduinoMenu::ArduinoMenu(const ArduinoMenuConfig& cfg)
    : _vane(cfg.vane), _io(cfg.io), _diag(cfg.diag), _storage(cfg.storage),
      _settingsStorage(cfg.settingsStorage), _settings(cfg.settings),
      _state(State::Main), _lastActivity(0), _lastCalibration(0) {
    _buffered = dynamic_cast<IBufferedDiagnostics*>(cfg.diag);
}

void ArduinoMenu::begin() {
    showMainMenu();
    _lastActivity = millis();
    _lastCalibration = _vane->lastCalibrationTimestamp();
}

void ArduinoMenu::update() {
    if (_io->hasInput()) {
        char c = _io->readInput();
        _lastActivity = millis();
        switch (_state) {
            case State::Main: handleMainInput(c); break;
            case State::LiveDisplay: _state=State::Main; showMainMenu(); break;
            default: break;
        }
    }

    switch (_state) {
        case State::LiveDisplay: updateLiveDisplay(); break;
        default: break;
    }

    if (millis() - _lastActivity > 30000 && _state != State::Main) {
        _state = State::Main;
        showMainMenu();
    }

    showStatusLine();
}

void ArduinoMenu::showStatusLine() {
    float dir = _vane->direction();
    unsigned long ago = (millis() - _lastCalibration)/60000UL;
    const char* st = statusText(_vane->calibrationStatus());
#ifdef ARDUINO
    renderStatusLineArduino(dir, st, ago);
#else
    renderStatusLineHost(dir, st, ago);
#endif
    clearExpiredMessage();
}

void ArduinoMenu::showMainMenu() {
#ifdef ARDUINO
    clearScreen();
    Serial.println();
    Serial.println("=== Wind Vane Menu ===");
    Serial.println("[D] Display direction ");
    Serial.println("[C] Calibrate        ");
    Serial.println("[G] Diagnostics      ");
    Serial.println("[S] Settings         ");
    Serial.println("[H] Help             ");
#else
    clearScreen();
    cout << "\n=== Wind Vane Menu ===" << endl;
    cout << "[D] Display direction " << endl;
    cout << "[C] Calibrate        " << endl;
    cout << "[G] Diagnostics      " << endl;
    cout << "[S] Settings         " << endl;
    cout << "[H] Help             " << endl;
    cout << "Choose option: " << endl;
#endif
}

void ArduinoMenu::handleMainInput(char c) {
    if (c == '\n' || c == '\r') { showMainMenu(); return; }
    switch(c) {
        case 'D': case 'd': handleDisplaySelection(); break;
        case 'C': case 'c': handleCalibrateSelection(); break;
        case 'G': case 'g': handleDiagnosticsSelection(); break;
        case 'S': case 's': handleSettingsSelectionMenu(); break;
        case 'H': case 'h': handleHelpSelection(); break;
        default: handleUnknownSelection(); break;
    }
}

void ArduinoMenu::updateLiveDisplay() {
    static unsigned long last = 0;
    if (millis() - last > 1000) {
        last = millis();
        float d = _vane->direction();
#ifdef ARDUINO
        Serial.print("\rDir: ");
        Serial.print(d,1);
        Serial.print("\xC2\xB0 (");
        Serial.print(compassPoint(d));
        Serial.print(")   \r");
#else
        cout << "\rDir: " << d << "\xC2\xB0 (" << compassPoint(d) << ")   \r" << std::flush;
#endif
    }
    if (_io->hasInput()) {
        _io->readInput();
        _state = State::Main;
        showMainMenu();
    }
}

const char* ArduinoMenu::statusText(CalibrationManager::CalibrationStatus st) const {
    switch (st) {
        case CalibrationManager::CalibrationStatus::NotStarted: return "Uncal";
        case CalibrationManager::CalibrationStatus::AwaitingStart: return "Awaiting";
        case CalibrationManager::CalibrationStatus::InProgress: return "Calibrating";
        case CalibrationManager::CalibrationStatus::Completed: return "OK";
    }
    return "Unknown";
}

void ArduinoMenu::renderStatusLineArduino(float dir, const char* statusStr, unsigned long ago) {
    char line[80];
    snprintf(line, sizeof(line), "\rDir:%6.1f\xC2\xB0 %-2s Status:%-10s Cal:%4lum", dir,
             compassPoint(dir), statusStr, ago);
    Serial.print(line);
    if (!_statusMsg.empty() && millis() < _msgExpiry) {
        if (_statusLevel != StatusLevel::Normal) Serial.print(" !! ");
        Serial.print(_statusMsg.c_str());
    }
    Serial.print("    \r");
}

void ArduinoMenu::renderStatusLineHost(float dir, const char* statusStr, unsigned long ago) {
    char line[80];
    snprintf(line, sizeof(line), "\rDir:%6.1f\xC2\xB0 %-2s Status:%-10s Cal:%4lum", dir,
             compassPoint(dir), statusStr, ago);
    std::cout << line;
    if (!_statusMsg.empty() && millis() < _msgExpiry) {
        const char* color=""; const char* reset="";
        if (_statusLevel == StatusLevel::Warning) { color="\033[33m"; reset="\033[0m"; }
        else if (_statusLevel == StatusLevel::Error) { color="\033[31m"; reset="\033[0m"; }
        std::cout << " " << color << _statusMsg << reset;
    }
    std::cout << "    \r" << std::flush;
}

void ArduinoMenu::clearExpiredMessage() {
    if (!_statusMsg.empty() && millis() >= _msgExpiry) {
        _statusMsg.clear();
        _statusLevel = StatusLevel::Normal;
    }
}

void ArduinoMenu::runCalibration() {
    if (_io->yesNoPrompt("Start calibration? (Y/N)")) {
        _vane->runCalibration();
        _lastCalibration = millis();
        if (_diag)
            _diag->info("Calibration completed");
        setStatusMessage("Calibration complete", StatusLevel::Normal);
    }
}

void ArduinoMenu::handleDisplaySelection() {
    _state = State::LiveDisplay;
    if (_vane->calibrationStatus() != CalibrationManager::CalibrationStatus::Completed)
        setStatusMessage("Warning: uncalibrated", StatusLevel::Warning);
#ifdef ARDUINO
    Serial.println("Live direction - press any key to return");
#else
    cout << "Live direction - press any key to return" << endl;
#endif
}

void ArduinoMenu::handleCalibrateSelection() {
    _state = State::Calibrate;
    runCalibration();
    _state = State::Main;
    showMainMenu();
}

void ArduinoMenu::handleDiagnosticsSelection() {
    _state = State::Diagnostics;
    DiagnosticsMenu menu(_vane, _io, _buffered, _diag);
    menu.show(_lastCalibration);
    _state = State::Main;
    showMainMenu();
}

void ArduinoMenu::handleSettingsSelectionMenu() {
    _state = State::Settings;
    SettingsMenu menu(_vane, _io, _storage, _settingsStorage, _settings);
    menu.run();
    _state = State::Main;
    showMainMenu();
}

void ArduinoMenu::handleHelpSelection() {
    _state = State::Help;
    showHelp();
    _state = State::Main;
    showMainMenu();
}

void ArduinoMenu::handleUnknownSelection() {
    setStatusMessage("Unknown option. Press [H] for help.", StatusLevel::Error);
}


void ArduinoMenu::showHelp() {
#ifdef ARDUINO
    Serial.println("--- Help ---");
    Serial.println("D: Live wind direction display");
    Serial.println("C: Start calibration routine");
    Serial.println("G: View diagnostics log");
    Serial.println("S: Settings and maintenance");
    Serial.println("H: Show this help text");
#endif
}

void ArduinoMenu::clearScreen() {
#ifdef ARDUINO
    Serial.print("\033[2J\033[H");
#else
    std::cout << "\033[2J\033[H";
#endif
}

void ArduinoMenu::setStatusMessage(const char* msg, StatusLevel lvl, unsigned long ms) {
    _statusMsg = msg;
    _statusLevel = lvl;
    _msgExpiry = millis() + ms;
}

