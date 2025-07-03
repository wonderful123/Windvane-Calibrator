#include "WindVaneCompass.h"
#include "../Platform/Platform.h"
#ifndef ARDUINO
#include <cstdio>
#endif

void WindVaneMenuPresenter::renderStatusLineArduino(const WindVaneStatus& st, const char* statusStr,
                               const std::string& msg, MenuStatusLevel level) const {
    char line[80];
    snprintf(line, sizeof(line),
             "\rDir:%6.1f\xC2\xB0 %-2s Status:%-10s Cal:%4lum", st.direction,
             compassPoint(st.direction), statusStr, st.minutesSinceCalibration);
    _out->write(line);
    if (!msg.empty()) {
        if (level != MenuStatusLevel::Normal) _out->write(" !! ");
        _out->write(msg.c_str());
    }
    _out->write("    \r");
}

void WindVaneMenuPresenter::renderStatusLineHost(const WindVaneStatus& st, const char* statusStr,
                             const std::string& msg, MenuStatusLevel level) const {
    char line[80];
    snprintf(line, sizeof(line),
             "\rDir:%6.1f\xC2\xB0 %-2s Status:%-10s Cal:%4lum", st.direction,
             compassPoint(st.direction), statusStr, st.minutesSinceCalibration);
    _out->write(line);
    if (!msg.empty()) {
        const char* color = "";
        const char* reset = "";
        if (level == MenuStatusLevel::Warning) {
            color = "\033[33m";
            reset = "\033[0m";
        } else if (level == MenuStatusLevel::Error) {
            color = "\033[31m";
            reset = "\033[0m";
        }
        _out->write(" ");
        _out->write(color);
        _out->write(msg.c_str());
        _out->write(reset);
    }
    _out->write("    \r");
}
