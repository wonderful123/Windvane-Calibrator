#pragma once
#include <IO/IIO.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include <Settings/SettingsManager.h>

class SettingsMenu {
 public:
  SettingsMenu(WindVane* vane, IUserIO* io, ICalibrationStorage* calibStorage,
               SettingsManager* settingsMgr, IOutput* out);

  void run();

  float readFloat();
  int readInt();

 private:
  WindVane* _vane;
  IUserIO* _io;
  ICalibrationStorage* _storage;
  SettingsManager* _settingsMgr;
  IOutput* _out;
};
