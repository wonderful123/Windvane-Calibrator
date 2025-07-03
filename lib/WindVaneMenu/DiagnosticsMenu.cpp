#include "DiagnosticsMenu.h"
#include <IO/IOutput.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#include <limits>
#include <chrono>
static unsigned long millis() {
  static auto start = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now() - start)
      .count();
}
#endif

DiagnosticsMenu::DiagnosticsMenu(WindVane* vane, IIOHandler* io,
                                 IBufferedDiagnostics* buffered,
                                 IDiagnostics* diag, IOutput* out)
    : _vane(vane), _io(io), _buffered(buffered), _diag(diag), _out(out) {}

void DiagnosticsMenu::show(unsigned long lastCalibration) {
#ifdef ARDUINO
    size_t index = 0;
    bool done = false;
    while (!done) {
        renderScreen(index, lastCalibration);
        char c = readCharBlocking();
        handleAction(c, index, done);
    }
#else
    (void)lastCalibration;
#endif
}

char DiagnosticsMenu::readCharBlocking() {
    while (!_io->hasInput())
        _io->waitMs(10);
    return _io->readInput();
}

void DiagnosticsMenu::renderScreen(size_t index, unsigned long lastCalibration) {
#ifdef ARDUINO
    char buf[32];
    _out->writeln("--- Diagnostics ---");
    _out->write("Calibration status: ");
    switch (_vane->calibrationStatus()) {
        case CalibrationManager::CalibrationStatus::Completed: _out->writeln("OK"); break;
        case CalibrationManager::CalibrationStatus::InProgress: _out->writeln("In progress"); break;
        case CalibrationManager::CalibrationStatus::AwaitingStart: _out->writeln("Awaiting"); break;
        default: _out->writeln("Not started"); break;
    }
    _out->write("Last calibration: ");
    snprintf(buf, sizeof(buf), "%lu", (millis() - lastCalibration)/60000UL);
    _out->write(buf);
    _out->writeln(" minutes ago");
    if (_buffered) {
        auto& hist = _buffered->history();
        for (size_t i=0;i<5 && index+i<hist.size();++i)
            _out->writeln(hist[index+i].c_str());
    }
    _out->writeln("[N]ext [P]rev [C]lear [T]est [B]ack");
#else
    (void)index; (void)lastCalibration;
#endif
}

void DiagnosticsMenu::handleAction(char c, size_t &index, bool &exit) {
    if (c=='N'||c=='n') {
        if (_buffered && index+5<_buffered->history().size()) index+=5;
    } else if (c=='P'||c=='p') {
        if (index>=5) index-=5;
    } else if (c=='C'||c=='c') {
        if (_buffered && _io->yesNoPrompt("Clear logs? (Y/N)")) {
            _buffered->clear();
            index=0;
        }
    } else if (c=='T'||c=='t') {
        selfTest();
    } else {
        exit = true;
    }
}

void DiagnosticsMenu::selfTest() {
    bool ok = true;
    float d = _vane->direction();
    if (d < 0 || d >= 360) ok = false;
    if (ok) {
        if (_diag) _diag->info("Self-test OK");
    } else {
        if (_diag) _diag->warn("Self-test failed");
    }
}
