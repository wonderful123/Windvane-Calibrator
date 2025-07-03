#pragma once
#include <WindVane.h>
#include <IO/IIOHandler.h>
#include <Storage/ICalibrationStorage.h>
#include <Settings/ISettingsStorage.h>
#include <Settings/SettingsData.h>
#include <IO/IOutput.h>

class SettingsMenu {
public:
    SettingsMenu(WindVane* vane, IIOHandler* io,
                 ICalibrationStorage* calibStorage,
                 ISettingsStorage* settingsStorage,
                 SettingsData* settings,
                 IOutput* out);
    void run();
private:
    WindVane* _vane;
    IIOHandler* _io;
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
