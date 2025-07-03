#pragma once
#include <WindVane.h>
#include <IO/IIOHandler.h>
#include <Diagnostics/IBufferedDiagnostics.h>
#include <Diagnostics/IDiagnostics.h>

class DiagnosticsMenu {
public:
    DiagnosticsMenu(WindVane* vane, IIOHandler* io,
                    IBufferedDiagnostics* buffered, IDiagnostics* diag);
    void show(unsigned long lastCalibration);
private:
    WindVane* _vane;
    IIOHandler* _io;
    IBufferedDiagnostics* _buffered;
    IDiagnostics* _diag;

    char readCharBlocking();
    void renderScreen(size_t index, unsigned long lastCalibration);
    void handleAction(char c, size_t &index, bool &exit);
    void selfTest();
};
