#include "ArduinoMenu.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <chrono>
#include <iostream>
using std::cout; using std::endl;
static unsigned long millis() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - start).count();
}
#endif

ArduinoMenu::ArduinoMenu(WindVane* vane, IIOHandler* io, IDiagnostics* diag)
    : _vane(vane), _io(io), _diag(diag), _state(State::Main),
      _lastActivity(0), _lastCalibration(0) {
    _buffered = dynamic_cast<BufferedDiagnostics*>(diag);
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
            case State::LiveDisplay: if (c=='q'||c=='Q') { _state=State::Main; showMainMenu(); } break;
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
    const char* statusStr = "Unknown";
    switch (_vane->calibrationStatus()) {
        case CalibrationManager::CalibrationStatus::NotStarted: statusStr = "Uncal"; break;
        case CalibrationManager::CalibrationStatus::AwaitingStart: statusStr = "Awaiting"; break;
        case CalibrationManager::CalibrationStatus::InProgress: statusStr = "Calibrating"; break;
        case CalibrationManager::CalibrationStatus::Completed: statusStr = "OK"; break;
    }
#ifdef ARDUINO
    Serial.print("\rDir: ");
    Serial.print(dir,1);
    Serial.print("\xC2\xB0 Status: ");
    Serial.print(statusStr);
    Serial.print(" Cal: ");
    Serial.print(ago);
    Serial.print("m    \r");
#else
    cout << "\rDir: " << dir << "\xC2\xB0 Status: " << statusStr
         << " Cal: " << ago << "m    \r" << std::flush;
#endif
}

void ArduinoMenu::showMainMenu() {
#ifdef ARDUINO
    Serial.println();
    Serial.println("=== Wind Vane Menu ===");
    Serial.println("[D] Display direction");
    Serial.println("[C] Calibrate" );
    Serial.println("[G] Diagnostics" );
    Serial.println("[S] Settings" );
    Serial.println("[H] Help" );
#else
    cout << "\n=== Wind Vane Menu ===" << endl;
    cout << "[D] Display direction" << endl;
    cout << "[C] Calibrate" << endl;
    cout << "[G] Diagnostics" << endl;
    cout << "[S] Settings" << endl;
    cout << "[H] Help" << endl;
    cout << "Choose option: " << endl;
#endif
}

void ArduinoMenu::handleMainInput(char c) {
    switch(c) {
        case 'D': case 'd':
            _state = State::LiveDisplay;
#ifdef ARDUINO
            Serial.println("Live direction - press Q to quit");
#else
            cout << "Live direction - press Q to quit" << endl;
#endif
            break;
        case 'C': case 'c':
            _state = State::Calibrate;
            runCalibration();
            _state = State::Main;
            showMainMenu();
            break;
        case 'G': case 'g':
            _state = State::Diagnostics;
            showDiagnostics();
            _state = State::Main;
            showMainMenu();
            break;
        case 'S': case 's':
            _state = State::Settings;
            settingsMenu();
            _state = State::Main;
            showMainMenu();
            break;
        case 'H': case 'h':
            _state = State::Help;
            showHelp();
            _state = State::Main;
            showMainMenu();
            break;
        default:
            break;
    }
}

void ArduinoMenu::updateLiveDisplay() {
    static unsigned long last = 0;
    if (millis() - last > 1000) {
        last = millis();
#ifdef ARDUINO
        Serial.print("\rDir: ");
        Serial.print(_vane->direction(),1);
        Serial.print("\xC2\xB0   \r");
#endif
    }
}

void ArduinoMenu::runCalibration() {
    if (_io->yesNoPrompt("Start calibration? (Y/N)")) {
        _vane->runCalibration();
        _lastCalibration = millis();
        if (_buffered)
            _buffered->info("Calibration completed");
    }
}

void ArduinoMenu::showDiagnostics() {
#ifdef ARDUINO
    Serial.println("--- Diagnostics ---");
    Serial.print("Calibration status: ");
    switch (_vane->calibrationStatus()) {
        case CalibrationManager::CalibrationStatus::Completed: Serial.println("OK"); break;
        case CalibrationManager::CalibrationStatus::InProgress: Serial.println("In progress"); break;
        case CalibrationManager::CalibrationStatus::AwaitingStart: Serial.println("Awaiting"); break;
        default: Serial.println("Not started"); break;
    }
    Serial.print("Last calibration: ");
    Serial.print((millis() - _lastCalibration)/60000UL);
    Serial.println(" minutes ago");
    if (_buffered) {
        for (const auto& msg : _buffered->history())
            Serial.println(msg.c_str());
    }
    Serial.println("Press any key to return");
    while (!_io->hasInput())
        _io->waitMs(10);
    _io->flushInput();
#endif
}

void ArduinoMenu::settingsMenu() {
#ifdef ARDUINO
    Serial.println("--- Settings ---");
    Serial.println("[R] Restore defaults");
    Serial.println("[B] Back");
    while (true) {
        while (!_io->hasInput()) _io->waitMs(10);
        char c = _io->readInput();
        if (c == 'R' || c == 'r') {
            if (_io->yesNoPrompt("Restore factory calibration? (Y/N)")) {
                _vane->runCalibration();
                _lastCalibration = millis();
            }
            break;
        }
        if (c == 'B' || c == 'b') break;
    }
#endif
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
