#pragma once
#include <IO/IIOHandler.h>
#include <IO/INumericReader.h>
#include <IO/IOutput.h>
#include <Settings/ISettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>

class SettingsMenu {
 public:
  SettingsMenu(WindVane* vane, IIOHandler* io, INumericReader* numeric,
               ICalibrationStorage* calibStorage,
               ISettingsStorage* settingsStorage, SettingsData* settings,
               IOutput* out);
  void run();

 private:
  WindVane* _vane;
  IIOHandler* _io;
  INumericReader* _numeric;
  ICalibrationStorage* _storage;
  ISettingsStorage* _settingsStorage;
  SettingsData* _settings;
  IOutput* _out;

  char readCharBlocking();
  float readFloat();
  int readInt();
  void showSummary();
  bool handleSelection(char c);
};
