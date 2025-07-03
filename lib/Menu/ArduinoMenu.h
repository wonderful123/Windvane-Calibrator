#pragma once
#include <Diagnostics/IBufferedDiagnostics.h>
#include <Diagnostics/IDiagnostics.h>
#include <IO/IIOHandler.h>
#include <IO/INumericReader.h>  // <-- ADD THIS INCLUDE
#include <IO/IOutput.h>
#include <Settings/ISettingsStorage.h>
#include <Settings/SettingsData.h>
#include <Storage/ICalibrationStorage.h>
#include <WindVane.h>

#include <string>

struct ArduinoMenuConfig {
  WindVane* vane{};
  IIOHandler* io{};
  IDiagnostics* diag{};
  IBufferedDiagnostics* bufferedDiag{};
  IOutput* out{};
  ICalibrationStorage* storage{};
  ISettingsStorage* settingsStorage{};
  SettingsData* settings{};
  INumericReader* numeric{};  // <-- ADD TO CONFIG, pass to SettingsMenu
};

class ArduinoMenu {
 public:
  explicit ArduinoMenu(const ArduinoMenuConfig& cfg);
  void begin();
  void update();

 private:
  WindVane* _vane;
  IIOHandler* _io;
  IDiagnostics* _diag;
  IBufferedDiagnostics* _buffered;
  IOutput* _out;
  ICalibrationStorage* _storage;
  ISettingsStorage* _settingsStorage;
  SettingsData* _settings;
  INumericReader* _numeric;  // <-- DECLARE _numeric

  enum class State {
    Main,
    LiveDisplay,
    Calibrate,
    Diagnostics,
    Settings,
    Help
  };
  State _state;
  unsigned long _lastActivity;
  unsigned long _lastCalibration;

  enum class StatusLevel { Normal, Warning, Error };
  std::string _statusMsg;
  StatusLevel _statusLevel{StatusLevel::Normal};
  unsigned long _msgExpiry{0};

  void showStatusLine();
  void showMainMenu();
  void handleMainInput(char c);
  void updateLiveDisplay();
  const char* statusText(CalibrationManager::CalibrationStatus st) const;
  void renderStatusLineArduino(float dir, const char* statusStr,
                               unsigned long ago);
  void renderStatusLineHost(float dir, const char* statusStr,
                            unsigned long ago);
  void clearExpiredMessage();
  void runCalibration();
  void handleDisplaySelection();
  void handleCalibrateSelection();
  void handleDiagnosticsSelection();
  void handleSettingsSelectionMenu();
  void handleHelpSelection();
  void handleUnknownSelection();
  void showHelp();
  void clearScreen();
  void setStatusMessage(const char* msg, StatusLevel lvl = StatusLevel::Normal,
                        unsigned long ms = 3000);
};
