#include "SettingsMenu.h"

SettingsMenu::SettingsMenu(WindVane* vane, IUserIO* io,
                           ICalibrationStorage* calibStorage,
                           SettingsManager* settingsMgr,
                           IOutput* out)
    : _vane(vane),
      _io(io),
      _storage(calibStorage),
      _settingsMgr(settingsMgr),
      _out(out) {}

void SettingsMenu::run() {
  _out->writeln("--- Settings Menu ---");
  _out->writeln("1. Change buffer size");
  _out->writeln("2. Save settings");
  _out->writeln("3. Back");
  _out->writeln("Choose option:");
  int opt = readInt();
  if (opt == 1) {
    _out->writeln("Enter new buffer size:");
    int newBuf = readInt();
    _settingsMgr->data().spin.bufferSize = newBuf;
    _out->writeln("Buffer size updated.");
  } else if (opt == 2) {
    _settingsMgr->save();
    _settingsMgr->apply(*_vane);
    _out->writeln("Settings saved.");
  }
}

float SettingsMenu::readFloat() {
  return _io ? _io->readFloat() : 0.0f;
}

int SettingsMenu::readInt() { return _io ? _io->readInt() : 0; }
