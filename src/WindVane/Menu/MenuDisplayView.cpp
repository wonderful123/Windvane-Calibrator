#include "MenuDisplayView.h"
#include "WindVaneCompass.h"
#include <cstdio>

MenuDisplayView::MenuDisplayView(IPlatform& platform, IUserIO& io, IOutput& out,
                                 MenuPresenter& presenter)
    : _platform(platform), _io(io), _out(out), _presenter(presenter) {}

bool MenuDisplayView::updateLiveDisplay(WindVane& vane, MenuState& state) const {
    if (_platform.millis() - state.lastDisplayUpdate > platform::TimeMs{1000}) {
        state.lastDisplayUpdate = _platform.millis();
        float d = vane.getDirection();
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

void MenuDisplayView::renderStatus(const WindVaneStatus& st, const char* statusStr,
                                   const std::string& msg, MenuStatusLevel level) const {
    _platform.renderStatusLine(_presenter, st, statusStr, msg, level);
}
