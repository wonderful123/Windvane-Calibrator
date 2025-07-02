#include "ArduinoMenu.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

ArduinoMenu::ArduinoMenu(WindVane* vane, IIOHandler* io, IDiagnostics* diag)
    : _vane(vane), _io(io), _diag(diag), _state(State::Main),
      _lastActivity(0), _lastCalibration(0) {}

void ArduinoMenu::begin() {
    showMainMenu();
    _lastActivity = millis();
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
#ifdef ARDUINO
    Serial.print("\rDir: ");
    Serial.print(dir,1);
    Serial.print("\xC2\xB0 Status: OK Cal: ");
    Serial.print(ago);
    Serial.print("m    \r");
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
    Serial.println("Choose option: " );
#endif
}

void ArduinoMenu::handleMainInput(char c) {
    switch(c) {
        case 'D': case 'd':
            _state = State::LiveDisplay;
            Serial.println("Live direction - press Q to quit");
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
    }
}

void ArduinoMenu::showDiagnostics() {
#ifdef ARDUINO
    Serial.println("Diagnostics not implemented");
#endif
}

void ArduinoMenu::settingsMenu() {
#ifdef ARDUINO
    Serial.println("Settings not implemented");
#endif
}

void ArduinoMenu::showHelp() {
#ifdef ARDUINO
    Serial.println("Use menu keys to operate.\nC: Calibrate, D: Display direction");
#endif
}
