#pragma once
#include <WindVane.h>
#include <IO/IIO.h>
#include <Diagnostics/IDiagnostics.h>
#include <Platform/IPlatform.h>

class DiagnosticsMenu {
public:
    DiagnosticsMenu(WindVane* vane, IIOHandler* io,
                    IBufferedDiagnostics* buffered, IDiagnostics* diag,
                    IOutput* out, IPlatform& platform);
    void show(unsigned long lastCalibration);
private:
    WindVane* _vane;
    IIOHandler* _io;
    IBufferedDiagnostics* _buffered;
    IDiagnostics* _diag;
    IOutput* _out;
    IPlatform* _platform;

    char readCharBlocking();
    void renderScreen(size_t index, unsigned long lastCalibration);
    void handleAction(char c, size_t &index, bool &exit);
    void selfTest();
};
