#include "SettingsMenu.h"

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
  _out->writeln("--- Settings Menu ---");
  // Simple example: change buffer size
  _out->writeln("1. Change buffer size");
  _out->writeln("2. Save settings");
  _out->writeln("3. Back");
  _out->writeln("Choose option:");
  int opt = readInt();
  if (opt == 1) {
    _out->writeln("Enter new buffer size:");
    int newBuf = readInt();
    _settings->spin.bufferSize = newBuf;
    _out->writeln("Buffer size updated.");
  } else if (opt == 2) {
    if (_settingsStorage) _settingsStorage->save(*_settings);
    _out->writeln("Settings saved.");
  }
  // add more settings as needed
}

float SettingsMenu::readFloat() {
  return _numeric ? _numeric->readFloat() : 0.0f;
}

int SettingsMenu::readInt() { return _numeric ? _numeric->readInt() : 0; }
