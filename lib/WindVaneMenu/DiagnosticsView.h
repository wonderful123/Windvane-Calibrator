#pragma once
#include <UI/IIO.h>
#include <Platform/IPlatform.h>
#include <Diagnostics/IDiagnostics.h>
#include <Calibration/CalibrationManager.h>
#include <deque>
#include <string>

struct DiagnosticsViewModel {
    CalibrationManager::CalibrationStatus status;
    platform::TimeMs minutesSinceCalibration;
    const std::deque<std::string>* history; // may be nullptr
};

class DiagnosticsView {
public:
    DiagnosticsView(IUserIO& io, IOutput& out, IPlatform& platform);
    char readCharBlocking() const;
    bool confirmClear() const;
    void render(const DiagnosticsViewModel& model, size_t index) const;
    IPlatform& platform() const { return _platform; }
private:
    IUserIO& _io;
    IOutput& _out;
    IPlatform& _platform;
};
