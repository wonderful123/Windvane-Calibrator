#include "MenuPresenter.h"
#include "WindVaneCompass.h"
#include <cstdio>

void MenuPresenter::renderStatusLine(const WindVaneStatus& st,
                                             const char* statusStr,
                                             const std::string& msg,
                                             MenuStatusLevel level,
                                             bool color) const {
    char line[80];
    snprintf(line, sizeof(line),
             "\rDir:%6.1f\xC2\xB0 %-2s Status:%-10s Cal:%4lum", st.direction,
             compassPoint(st.direction), statusStr, st.minutesSinceCalibration);
    _out->write(line);
    if (!msg.empty()) {
        const char* start = "";
        const char* end = "";
        if (color) {
            if (level == MenuStatusLevel::Warning) {
                start = "\033[33m";
                end = "\033[0m";
            } else if (level == MenuStatusLevel::Error) {
                start = "\033[31m";
                end = "\033[0m";
            }
        } else if (level != MenuStatusLevel::Normal) {
            _out->write(" !! ");
        } else {
            _out->write(" ");
        }
        _out->write(start);
        _out->write(msg.c_str());
        _out->write(end);
    }
    _out->write("    \r");
}
