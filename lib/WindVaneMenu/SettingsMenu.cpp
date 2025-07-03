#include "SettingsMenu.h"
#include <Storage/StorageResult.h>

SettingsMenu::SettingsMenu(WindVane& vane, IUserIO& io,
                           ICalibrationStorage& calibStorage,
                           SettingsManager& settingsMgr,
                           IOutput& out)
    : _vane(vane),
      _io(io),
      _storage(calibStorage),
      _settingsMgr(settingsMgr),
      _out(out) {}

void SettingsMenu::run() const {
  _out.writeln("--- Settings Menu ---");
  _out.writeln("1. Change buffer size");
  _out.writeln("2. Save settings");
  _out.writeln("3. Back");
  _out.writeln("Choose option:");
  int opt = readInt();
  if (opt == 1) {
    _out.writeln("Enter new buffer size:");
    int newBuf = readInt();
    _settingsMgr.setSpinBufferSize(newBuf);
    _out.writeln("Buffer size updated.");
  } else if (opt == 2) {
    StorageResult res = _settingsMgr.save();
    if (res.ok()) {
      _settingsMgr.apply(_vane);
      _out.writeln("Settings saved.");
    } else {
      _out.writeln("Failed to save settings");
    }
  }
}

float SettingsMenu::readFloat() const {
  return _io.readFloat();
}

int SettingsMenu::readInt() const { return _io.readInt(); }
