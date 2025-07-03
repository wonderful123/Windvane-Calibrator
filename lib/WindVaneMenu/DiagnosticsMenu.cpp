#include "DiagnosticsMenu.h"
#include "DiagnosticsView.h"
#include <iostream>
#include <limits>

DiagnosticsMenu::DiagnosticsMenu(
    WindVane& vane,
    std::optional<std::reference_wrapper<IBufferedDiagnostics>> buffered,
    DiagnosticsView& view, IDiagnostics& diag)
    : _vane(vane), _buffered(std::move(buffered)), _view(view), _diag(diag) {}

void DiagnosticsMenu::show(platform::TimeMs lastCalibration) const {
    size_t index = 0;
    bool done = false;
    while (!done) {
        renderScreen(index, lastCalibration);
        char c = readCharBlocking();
        ActionResult r = handleAction(c, index);
        index = r.index;
        done = r.exit;
    }
}

char DiagnosticsMenu::readCharBlocking() const {
    return _view.readCharBlocking();
}

void DiagnosticsMenu::renderScreen(size_t index, platform::TimeMs lastCalibration) const {
    DiagnosticsViewModel model;
    model.status = _vane.getCalibrationStatus();
    model.minutesSinceCalibration = _view.platform().millis() - lastCalibration;
    if (_buffered)
        model.history = &_buffered->get().history();
    else
        model.history = nullptr;
    _view.render(model, index);
}

DiagnosticsMenu::ActionResult DiagnosticsMenu::handleAction(char c, size_t index) const {
    ActionResult out{index, false};
    if (c=='N'||c=='n') {
        if (_buffered && index+5<_buffered->get().history().size()) out.index+=5;
    } else if (c=='P'||c=='p') {
        if (index>=5) out.index-=5;
    } else if (c=='C'||c=='c') {
        if (_buffered && _view.confirmClear()) {
            _buffered->get().clear();
            out.index=0;
        }
    } else if (c=='T'||c=='t') {
        SelfTestStatus st = selfTest();
        if (st == SelfTestStatus::Ok)
            _diag.info("Self-test OK");
        else
            _diag.warn("Self-test failed");
    } else {
        out.exit = true;
    }
    return out;
}

DiagnosticsMenu::SelfTestStatus DiagnosticsMenu::selfTest() const {
    bool ok = true;
    float d = _vane.getDirection();
    if (d < 0 || d >= 360) ok = false;
    return ok ? SelfTestStatus::Ok : SelfTestStatus::Failed;
}
