#include "MenuDisplayController.h"
#include "WindVaneCompass.h"
#include <cstdio>

MenuDisplayController::MenuDisplayController(
    IPlatform& platform, IUserIO& io, IOutput& out,
    MenuPresenter& presenter, MenuLogic& logic)
    : _platform(platform),
      _io(io),
      _out(out),
      _presenter(presenter),
      _logic(logic),
      _lastActivity(platform::TimeMs{0}),
      _lastCalibration(platform::TimeMs{0}),
      _statusLevel(MenuStatusLevel::Normal),
      _msgExpiry(0) {}

void MenuDisplayController::begin(WindVane& vane) {
    _lastActivity = _platform.millis();
    _lastCalibration = vane.lastCalibrationTimestamp();
}

void MenuDisplayController::onInput() { _lastActivity = _platform.millis(); }

bool MenuDisplayController::checkTimeout() const {
    return _platform.millis() - _lastActivity > platform::TimeMs{30000};
}

bool MenuDisplayController::updateLiveDisplay(WindVane& vane) const {
    static platform::TimeMs last = platform::TimeMs{0};
    if (_platform.millis() - last > platform::TimeMs{1000}) {
        last = _platform.millis();
        float d = vane.direction();
        char buf[64];
        snprintf(buf, sizeof(buf), "\rDir: %.1f\xC2\xB0 (%s)   \r", d, compassPoint(d));
        _out.write(buf);
    }
    if (_io.hasInput()) {
        _io.readInput();
        return true;
    }
    return false;
}

void MenuDisplayController::showStatusLine(WindVane& vane) {
    WindVaneStatus st = _logic.queryStatus(&vane, _lastCalibration, _platform);
    const char* statusStr = _logic.statusText(st.calibrationStatus);
    _platform.renderStatusLine(_presenter, st, statusStr, _statusMsg, _statusLevel);
    clearExpiredMessage();
}

void MenuDisplayController::setStatusMessage(const char* msg,
                                                     MenuStatusLevel lvl,
                                                     platform::TimeMs ms) {
    _statusMsg = msg;
    _statusLevel = lvl;
    _msgExpiry = _platform.millis() + ms;
}

void MenuDisplayController::recordCalibration() {
    _lastCalibration = _platform.millis();
}

void MenuDisplayController::clearExpiredMessage() {
    if (!_statusMsg.empty() && _platform.millis() >= _msgExpiry) {
        _statusMsg.clear();
        _statusLevel = MenuStatusLevel::Normal;
    }
}

