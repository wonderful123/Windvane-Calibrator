#pragma once
#include <IO/IIOHandler.h>
#include <IO/INumericReader.h>
#include <IO/IOutput.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>
#include <Settings/SettingsManager.h>

class SettingsMenu {
 public:
  SettingsMenu(WindVane* vane, IIOHandler* io, INumericReader* numeric,
               ICalibrationStorage* calibStorage, SettingsManager* settingsMgr,
               IOutput* out);

  void run();

  float readFloat();
  int readInt();

 private:
  WindVane* _vane;
  IIOHandler* _io;
  INumericReader* _numeric;
  ICalibrationStorage* _storage;
  SettingsManager* _settingsMgr;
  IOutput* _out;
};
