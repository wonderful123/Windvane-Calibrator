#include "WindVaneMenuDisplayController.h"
#include "WindVaneCompass.h"
#include <cstdio>

WindVaneMenuDisplayController::WindVaneMenuDisplayController(
    IPlatform& platform, IUserIO& io, IOutput& out,
    WindVaneMenuPresenter& presenter, WindVaneMenuLogic& logic)
    : _platform(platform),
      _io(io),
      _out(out),
      _presenter(presenter),
      _logic(logic),
      _lastActivity(0),
      _lastCalibration(0),
      _statusLevel(MenuStatusLevel::Normal),
      _msgExpiry(0) {}

void WindVaneMenuDisplayController::begin(WindVane& vane) {
    _lastActivity = _platform.millis();
    _lastCalibration = vane.lastCalibrationTimestamp();
}

void WindVaneMenuDisplayController::onInput() { _lastActivity = _platform.millis(); }

bool WindVaneMenuDisplayController::checkTimeout() const {
    return _platform.millis() - _lastActivity > 30000;
}

bool WindVaneMenuDisplayController::updateLiveDisplay(WindVane& vane) {
    static unsigned long last = 0;
    if (_platform.millis() - last > 1000) {
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

void WindVaneMenuDisplayController::showStatusLine(WindVane& vane) {
    WindVaneStatus st = _logic.queryStatus(&vane, _lastCalibration, _platform);
    const char* statusStr = _logic.statusText(st.calibrationStatus);
    _platform.renderStatusLine(_presenter, st, statusStr, _statusMsg, _statusLevel);
    clearExpiredMessage();
}

void WindVaneMenuDisplayController::setStatusMessage(const char* msg,
                                                     MenuStatusLevel lvl,
                                                     unsigned long ms) {
    _statusMsg = msg;
    _statusLevel = lvl;
    _msgExpiry = _platform.millis() + ms;
}

void WindVaneMenuDisplayController::recordCalibration() {
    _lastCalibration = _platform.millis();
}

void WindVaneMenuDisplayController::clearExpiredMessage() {
    if (!_statusMsg.empty() && _platform.millis() >= _msgExpiry) {
        _statusMsg.clear();
        _statusLevel = MenuStatusLevel::Normal;
    }
}

