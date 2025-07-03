#include "MenuDisplayController.h"
#include "WindVaneCompass.h"
#include <cstdio>

MenuDisplayController::MenuDisplayController(
    IPlatform& platform, MenuDisplayView& view,
    MenuLogic& logic, MenuState& state)
    : _platform(platform),
      _view(view),
      _logic(logic),
      _state(state) {}

void MenuDisplayController::begin(WindVane& vane) {
    _state.lastActivity = _platform.millis();
    _state.lastCalibration = vane.getLastCalibrationTimestamp();
}

void MenuDisplayController::onInput() { _state.lastActivity = _platform.millis(); }

bool MenuDisplayController::checkTimeout() const {
    return _platform.millis() - _state.lastActivity > platform::TimeMs{30000};
}

bool MenuDisplayController::updateLiveDisplay(WindVane& vane) const {
    return _view.updateLiveDisplay(vane, _state);
}

void MenuDisplayController::showStatusLine(WindVane& vane) {
    WindVaneStatus st = _logic.queryStatus(&vane, _state.lastCalibration, _platform);
    const char* statusStr = _logic.statusText(st.calibrationStatus);
    _view.renderStatus(st, statusStr, _state.statusMsg, _state.statusLevel);
    clearExpiredMessage();
}

void MenuDisplayController::setStatusMessage(const char* msg,
                                                     MenuStatusLevel lvl,
                                                     platform::TimeMs ms) {
    _state.statusMsg = msg;
    _state.statusLevel = lvl;
    _state.msgExpiry = _platform.millis() + ms;
}

void MenuDisplayController::recordCalibration() {
    _state.lastCalibration = _platform.millis();
}

void MenuDisplayController::clearExpiredMessage() {
    if (!_state.statusMsg.empty() && _platform.millis() >= _state.msgExpiry) {
        _state.statusMsg.clear();
        _state.statusLevel = MenuStatusLevel::Normal;
    }
}

