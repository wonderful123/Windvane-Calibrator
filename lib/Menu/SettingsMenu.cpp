#include "SettingsMenu.h"
#include <IO/IOutput.h>
#include "INumericReader.h"
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
                           SettingsData* settings,
                           IOutput* out)
    : _vane(vane), _io(io), _storage(calibStorage),
      _settingsStorage(settingsStorage), _settings(settings), _out(out) {}

void SettingsMenu::run() {
#ifdef ARDUINO
    showSummary();
    _out->writeln("[1] Threshold  [2] Detents  [3] Smoothing");
    _out->writeln("[F] Factory reset  [B] Back");
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
    if (auto num = dynamic_cast<INumericReader*>(_io))
        return num->readFloat();
    return 0.0f;
}

int SettingsMenu::readInt() {
    if (auto num = dynamic_cast<INumericReader*>(_io))
        return num->readInt();
    return 0;
}

void SettingsMenu::showSummary() {
#ifdef ARDUINO
    _out->writeln("--- Settings ---");
    char buf[32];
    snprintf(buf, sizeof(buf), "%f", _settings->spin.threshold);
    _out->write("Threshold: "); _out->writeln(buf);
    snprintf(buf, sizeof(buf), "%d", _settings->spin.expectedPositions);
    _out->write("Detents: "); _out->writeln(buf);
    snprintf(buf, sizeof(buf), "%d", _settings->spin.bufferSize);
    _out->write("Smoothing: "); _out->writeln(buf);
#endif
}

bool SettingsMenu::handleSelection(char c) {
#ifdef ARDUINO
    if (c == '1') {
        _out->writeln("Enter new threshold:");
        float v = readFloat();
        if (_io->yesNoPrompt("Apply? (Y/N)")) _settings->spin.threshold = v;
    } else if (c=='2') {
        _out->writeln("Enter detent count:");
        int v = readInt();
        if (_io->yesNoPrompt("Apply? (Y/N)")) _settings->spin.expectedPositions = v;
    } else if (c=='3') {
        _out->writeln("Enter smoothing size:");
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
