#include "MenuDisplayView.h"
#include "WindVaneCompass.h"
#include <cstdio>

MenuDisplayView::MenuDisplayView(IPlatform& platform, IUserIO& io, IOutput& out,
                                 MenuPresenter& presenter)
    : _platform(platform), _io(io), _out(out), _presenter(presenter) {}

bool MenuDisplayView::updateLiveDisplay(WindVane& vane) const {
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

void MenuDisplayView::renderStatus(const WindVaneStatus& st, const char* statusStr,
                                   const std::string& msg, MenuStatusLevel level) const {
    _platform.renderStatusLine(_presenter, st, statusStr, msg, level);
}
