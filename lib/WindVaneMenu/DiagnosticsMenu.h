#pragma once
#include <WindVane.h>
#include <UI/IIO.h>
#include <Diagnostics/IDiagnostics.h>
#include <Platform/IPlatform.h>

class DiagnosticsMenu {
public:
    DiagnosticsMenu(WindVane* vane, IUserIO* io,
                    IBufferedDiagnostics* buffered, IDiagnostics* diag,
                    IOutput* out, IPlatform& platform);
    void show(platform::TimeMs lastCalibration) const;
    enum class SelfTestStatus { Ok, Failed };
private:
    WindVane* _vane;
    IUserIO* _io;
    IBufferedDiagnostics* _buffered;
    IDiagnostics* _diag;
    IOutput* _out;
    IPlatform* _platform;

    char readCharBlocking() const;
    void renderScreen(size_t index, platform::TimeMs lastCalibration) const;
    void handleAction(char c, size_t &index, bool &exit) const;
    SelfTestStatus selfTest() const;
};
