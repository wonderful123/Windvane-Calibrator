#include "SettingsMenu.h"
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#include <limits>
#include <chrono>
#endif

SettingsMenu::SettingsMenu(WindVane* vane, IIOHandler* io,
                           ICalibrationStorage* calibStorage,
                           ISettingsStorage* settingsStorage,
                           SettingsData* settings)
    : _vane(vane), _io(io), _storage(calibStorage),
      _settingsStorage(settingsStorage), _settings(settings) {}

void SettingsMenu::run() {
#ifdef ARDUINO
    showSummary();
    Serial.println("[1] Threshold  [2] Detents  [3] Smoothing");
    Serial.println("[F] Factory reset  [B] Back");
    bool done = false;
    while (!done) {
        char c = readCharBlocking();
        done = handleSelection(c);
    }
    if (_settingsStorage) _settingsStorage->save(*_settings);
    _vane->setCalibrationConfig(_settings->spin);
#endif
}

char SettingsMenu::readCharBlocking() {
    while (!_io->hasInput())
        _io->waitMs(10);
    return _io->readInput();
}

float SettingsMenu::readFloat() {
#ifdef ARDUINO
    while (!Serial.available()) _io->waitMs(10);
    float v = Serial.parseFloat();
    Serial.readStringUntil('\n');
    return v;
#else
    float v; std::cin >> v; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v;
#endif
}

int SettingsMenu::readInt() {
#ifdef ARDUINO
    while (!Serial.available()) _io->waitMs(10);
    int v = Serial.parseInt();
    Serial.readStringUntil('\n');
    return v;
#else
    int v; std::cin >> v; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v;
#endif
}

void SettingsMenu::showSummary() {
#ifdef ARDUINO
    Serial.println("--- Settings ---");
    Serial.print("Threshold: "); Serial.println(_settings->spin.threshold);
    Serial.print("Detents: "); Serial.println(_settings->spin.expectedPositions);
    Serial.print("Smoothing: "); Serial.println(_settings->spin.bufferSize);
#endif
}

bool SettingsMenu::handleSelection(char c) {
#ifdef ARDUINO
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
        return true;
    }
#else
    (void)c;
#endif
    return false;
}
