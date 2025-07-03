#include "DiagnosticsMenu.h"
#include <Platform/Platform.h>
#include <UI/IIO.h>
#include <iostream>
#include <limits>

DiagnosticsMenu::DiagnosticsMenu(WindVane& vane, IUserIO& io,
                                 IBufferedDiagnostics* buffered,
                                 IDiagnostics& diag, IOutput& out,
                                 IPlatform& platform)
    : _vane(vane), _io(io), _buffered(buffered), _diag(diag), _out(out),
      _platform(platform) {}

void DiagnosticsMenu::show(platform::TimeMs lastCalibration) const {
    size_t index = 0;
    bool done = false;
    while (!done) {
        renderScreen(index, lastCalibration);
        char c = readCharBlocking();
        handleAction(c, index, done);
    }
}

char DiagnosticsMenu::readCharBlocking() const {
    while (!_io.hasInput())
        _io.waitMs(10);
    return _io.readInput();
}

void DiagnosticsMenu::renderScreen(size_t index, platform::TimeMs lastCalibration) const {
    char buf[32];
    _out.writeln("--- Diagnostics ---");
    _out.write("Calibration status: ");
    switch (_vane.calibrationStatus()) {
        case CalibrationManager::CalibrationStatus::Completed: _out.writeln("OK"); break;
        case CalibrationManager::CalibrationStatus::InProgress: _out.writeln("In progress"); break;
        case CalibrationManager::CalibrationStatus::AwaitingStart: _out.writeln("Awaiting"); break;
        default: _out.writeln("Not started"); break;
    }
    _out.write("Last calibration: ");
    snprintf(buf, sizeof(buf), "%lu",
             platform::toEmbedded(_platform.millis() - lastCalibration) / 60000UL);
    _out.write(buf);
    _out.writeln(" minutes ago");
    if (_buffered) {
        auto& hist = _buffered->history();
        for (size_t i=0;i<5 && index+i<hist.size();++i)
            _out.writeln(hist[index+i].c_str());
    }
    _out.writeln("[N]ext [P]rev [C]lear [T]est [B]ack");
}

void DiagnosticsMenu::handleAction(char c, size_t &index, bool &exit) const {
    if (c=='N'||c=='n') {
        if (_buffered && index+5<_buffered->history().size()) index+=5;
    } else if (c=='P'||c=='p') {
        if (index>=5) index-=5;
    } else if (c=='C'||c=='c') {
        if (_buffered && _io.yesNoPrompt("Clear logs? (Y/N)")) {
            _buffered->clear();
            index=0;
        }
    } else if (c=='T'||c=='t') {
        SelfTestStatus st = selfTest();
        if (st == SelfTestStatus::Ok)
            _diag.info("Self-test OK");
        else
            _diag.warn("Self-test failed");
    } else {
        exit = true;
    }
}

DiagnosticsMenu::SelfTestStatus DiagnosticsMenu::selfTest() const {
    bool ok = true;
    float d = _vane.direction();
    if (d < 0 || d >= 360) ok = false;
    return ok ? SelfTestStatus::Ok : SelfTestStatus::Failed;
}
