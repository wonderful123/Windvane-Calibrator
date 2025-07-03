#include "MenuDisplayController.h"
#include "WindVaneCompass.h"
#include <cstdio>

MenuDisplayController::MenuDisplayController(
    IPlatform& platform, MenuDisplayView& view,
    MenuLogic& logic)
    : _platform(platform),
      _view(view),
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
    return _view.updateLiveDisplay(vane);
}

void MenuDisplayController::showStatusLine(WindVane& vane) {
    WindVaneStatus st = _logic.queryStatus(&vane, _lastCalibration, _platform);
    const char* statusStr = _logic.statusText(st.calibrationStatus);
    _view.renderStatus(st, statusStr, _statusMsg, _statusLevel);
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

