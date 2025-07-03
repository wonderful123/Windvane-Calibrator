#pragma once
#include <Diagnostics/IBufferedDiagnostics.h>
#include <Diagnostics/IDiagnostics.h>
#include <IO/IIOHandler.h>
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
  IBufferedDiagnostics* bufferedDiag{};  // NEW FIELD
  IOutput* out{};
  ICalibrationStorage* storage{nullptr};
  ISettingsStorage* settingsStorage{nullptr};
  SettingsData* settings{nullptr};
};

class ArduinoMenu {
 public:
  explicit ArduinoMenu(const ArduinoMenuConfig& cfg);
  void begin();
  void update();

 private:
  enum class State {
    Main,
    LiveDisplay,
    Calibrate,
    Diagnostics,
    Settings,
    Help
  };

  WindVane* _vane;
  IIOHandler* _io;
  IDiagnostics* _diag;
  IBufferedDiagnostics* _buffered{nullptr};
  IOutput* _out;
  ICalibrationStorage* _storage{nullptr};
  ISettingsStorage* _settingsStorage{nullptr};
  SettingsData* _settings{nullptr};
  std::string _statusMsg;
  enum class StatusLevel { Normal, Warning, Error };
  StatusLevel _statusLevel{StatusLevel::Normal};
  unsigned long _msgExpiry{0};
  State _state;
  unsigned long _lastActivity;
  unsigned long _lastCalibration;

  void showMainMenu();
  void showStatusLine();
  const char* statusText(CalibrationManager::CalibrationStatus st) const;
  void renderStatusLineArduino(float dir, const char* statusStr,
                               unsigned long ago);
  void renderStatusLineHost(float dir, const char* statusStr,
                            unsigned long ago);
  void clearExpiredMessage();
  void handleMainInput(char c);
  void handleDisplaySelection();
  void handleCalibrateSelection();
  void handleDiagnosticsSelection();
  void handleSettingsSelectionMenu();
  void handleHelpSelection();
  void handleUnknownSelection();
  void updateLiveDisplay();
  void runCalibration();
  void showHelp();
  void setStatusMessage(const char* msg, StatusLevel lvl = StatusLevel::Normal,
                        unsigned long ms = 3000);
  void clearScreen();
};
