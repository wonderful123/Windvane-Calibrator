#include "ArduinoMenu.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <chrono>
#include <iostream>
#include <limits>
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

ArduinoMenu::ArduinoMenu(WindVane* vane, IIOHandler* io, IDiagnostics* diag,
                         ICalibrationStorage* storage,
                         ISettingsStorage* settingsStorage,
                         SettingsData* settings)
    : _vane(vane), _io(io), _diag(diag), _storage(storage),
      _settingsStorage(settingsStorage), _settings(settings),
      _state(State::Main), _lastActivity(0), _lastCalibration(0) {
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
    Serial.print("\xC2\xB0 (");
    Serial.print(compassPoint(dir));
    Serial.print(") Status: ");
    Serial.print(statusStr);
    Serial.print(" Cal: ");
    Serial.print(ago);
    if (!_statusMsg.empty() && millis() < _msgExpiry) {
        Serial.print(" ");
        Serial.print(_statusMsg.c_str());
    }
    Serial.print("m    \r");
    if (!_statusMsg.empty() && millis() >= _msgExpiry) _statusMsg.clear();
#else
    cout << "\rDir: " << dir << "\xC2\xB0 (" << compassPoint(dir) << ") Status: " << statusStr
         << " Cal: " << ago;
    if (!_statusMsg.empty() && millis() < _msgExpiry)
        cout << " " << _statusMsg.c_str();
    cout << "m    \r" << std::flush;
    if (!_statusMsg.empty() && millis() >= _msgExpiry) _statusMsg.clear();
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
    if (c == '\n' || c == '\r') { showMainMenu(); return; }
    switch(c) {
        case 'D': case 'd':
            _state = State::LiveDisplay;
            if (_vane->calibrationStatus() != CalibrationManager::CalibrationStatus::Completed)
                setStatusMessage("Warning: uncalibrated");
#ifdef ARDUINO
            Serial.println("Live direction - press any key to return");
#else
            cout << "Live direction - press any key to return" << endl;
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
            setStatusMessage("Unknown option. Press [H] for help.");
            break;
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
    size_t index = 0;
    while (true) {
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
            auto& hist = _buffered->history();
            for (size_t i=0;i<5 && index+i<hist.size();++i)
                Serial.println(hist[index+i].c_str());
        }
        Serial.println("[N]ext [P]rev [C]lear [T]est [B]ack");
        while (!_io->hasInput()) _io->waitMs(10);
        char c = _io->readInput();
        if (c=='N'||c=='n') { if (_buffered && index+5<_buffered->history().size()) index+=5; }
        else if (c=='P'||c=='p') { if (index>=5) index-=5; }
        else if (c=='C'||c=='c') { if (_buffered && _io->yesNoPrompt("Clear logs? (Y/N)")) _buffered->clear(); index=0; }
        else if (c=='T'||c=='t') { selfTest(); }
        else break;
    }
#endif
}

void ArduinoMenu::settingsMenu() {
#ifdef ARDUINO
    Serial.println("--- Settings ---");
    Serial.print("Threshold: "); Serial.println(_settings->spin.threshold);
    Serial.print("Detents: "); Serial.println(_settings->spin.expectedPositions);
    Serial.print("Smoothing: "); Serial.println(_settings->spin.bufferSize);
    Serial.println("[1] Threshold  [2] Detents  [3] Smoothing");
    Serial.println("[F] Factory reset  [B] Back");
    while (true) {
        while (!_io->hasInput()) _io->waitMs(10);
        char c = _io->readInput();
        if (c == '1') {
            Serial.println("Enter new threshold:");
            float v = readFloat();
            if (_io->yesNoPrompt("Apply? (Y/N)")) _settings->spin.threshold = v;
        } else if (c=='2') {
            Serial.println("Enter detent count:");
            int v = readInt();
            if (_io->yesNoPrompt("Apply? (Y/N)")) _settings->spin.expectedPositions = v;
        } else if (c=='3') {
            Serial.println("Enter smoothing size:");
            int v = readInt();
            if (_io->yesNoPrompt("Apply? (Y/N)")) _settings->spin.bufferSize = v;
        } else if (c=='F' || c=='f') {
            if (_io->yesNoPrompt("Factory reset? (Y/N)")) {
                if (_storage) _storage->clear();
                *_settings = SettingsData();
            }
        } else if (c=='B' || c=='b' || c=='\n' || c=='\r') {
            break;
        }
    }
    if (_settingsStorage) _settingsStorage->save(*_settings);
    _vane->setCalibrationConfig(_settings->spin);
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

void ArduinoMenu::setStatusMessage(const char* msg, unsigned long ms) {
    _statusMsg = msg;
    _msgExpiry = millis() + ms;
}

float ArduinoMenu::readFloat() {
#ifdef ARDUINO
    while (!Serial.available()) _io->waitMs(10);
    float v = Serial.parseFloat();
    Serial.readStringUntil('\n');
    return v;
#else
    float v; std::cin >> v; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v;
#endif
}

int ArduinoMenu::readInt() {
#ifdef ARDUINO
    while (!Serial.available()) _io->waitMs(10);
    int v = Serial.parseInt();
    Serial.readStringUntil('\n');
    return v;
#else
    int v; std::cin >> v; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v;
#endif
}

void ArduinoMenu::selfTest() {
    bool ok = true;
    float d = _vane->direction();
    if (d < 0 || d >= 360) ok = false;
    if (ok) {
        if (_diag) _diag->info("Self-test OK");
    } else {
        if (_diag) _diag->warn("Self-test failed");
    }
}
