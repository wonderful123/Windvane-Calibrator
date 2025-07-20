#include "DiagnosticsView.h"
#include <cstdio>

DiagnosticsView::DiagnosticsView(IUserIO& io, IOutput& out, IPlatform& platform)
    : _io(io), _out(out), _platform(platform) {}

char DiagnosticsView::readCharBlocking() const {
    while (!_io.hasInput())
        _io.waitMs(platform::TimeMs{10});
    return _io.readInput();
}

bool DiagnosticsView::confirmClear() const {
    return _io.yesNoPrompt("Clear logs? (Y/N)");
}

void DiagnosticsView::render(const DiagnosticsViewModel& model, size_t index) const {
    char buf[32];
    _out.writeln("--- Diagnostics ---");
    _out.write("Calibration status: ");
    switch (model.status) {
        case CalibrationManager::CalibrationStatus::Completed: _out.writeln("OK"); break;
        case CalibrationManager::CalibrationStatus::InProgress: _out.writeln("In progress"); break;
        case CalibrationManager::CalibrationStatus::AwaitingStart: _out.writeln("Awaiting"); break;
        default: _out.writeln("Not started"); break;
    }
    _out.write("Last calibration: ");
    snprintf(buf, sizeof(buf), "%lu", platform::toEmbedded(model.minutesSinceCalibration) / 60000UL);
    _out.write(buf);
    _out.writeln(" minutes ago");
    if (model.history) {
        const auto& hist = *model.history;
        for (size_t i = 0; i < 5 && index + i < hist.size(); ++i)
            _out.writeln(hist[index + i].c_str());
    }
    _out.writeln("[N]ext [P]rev [C]lear [T]est [B]ack");
}
