#include "SettingsMenu.h"

#include <IO/IOutput.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <chrono>
#include <iostream>
#include <limits>
#endif

SettingsMenu::SettingsMenu(WindVane* vane, IIOHandler* io,
                           INumericReader* numeric,
                           ICalibrationStorage* calibStorage,
                           ISettingsStorage* settingsStorage,
                           SettingsData* settings, IOutput* out)
    : _vane(vane),
      _io(io),
      _numeric(numeric),
      _storage(calibStorage),
      _settingsStorage(settingsStorage),
      _settings(settings),
      _out(out) {}

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
  while (!_io->hasInput()) _io->waitMs(10);
  return _io->readInput();
}

float SettingsMenu::readFloat() {
  if (_numeric) return _numeric->readFloat();
  return 0.0f;
}

int SettingsMenu::readInt() {
  if (_numeric) return _numeric->readInt();
  return 0;
}

// ... rest of file unchanged ...
