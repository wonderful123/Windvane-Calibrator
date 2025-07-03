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
    void show(platform::TimeMs lastCalibration);
private:
    WindVane* _vane;
    IUserIO* _io;
    IBufferedDiagnostics* _buffered;
    IDiagnostics* _diag;
    IOutput* _out;
    IPlatform* _platform;

    char readCharBlocking();
    void renderScreen(size_t index, platform::TimeMs lastCalibration);
    void handleAction(char c, size_t &index, bool &exit);
    void selfTest();
};
