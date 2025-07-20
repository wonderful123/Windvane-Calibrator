#pragma once
#include <UI/IIO.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include <Storage/Settings/SettingsManager.h>

class SettingsMenu {
 public:
  SettingsMenu(WindVane& vane, IUserIO& io, ICalibrationStorage& calibStorage,
               SettingsManager& settingsMgr, IOutput& out);

  void run() const;

  float readFloat() const;
  int readInt() const;

 private:
  WindVane& _vane;
  IUserIO& _io;
  ICalibrationStorage& _storage;
  SettingsManager& _settingsMgr;
  IOutput& _out;
};
