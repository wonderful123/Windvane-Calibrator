#pragma once
#include "MenuPresenter.h"
#include "MenuLogic.h"
#include "WindVaneStatus.h"
#include <Platform/IPlatform.h>
#include <Platform/TimeUtils.h>
#include "MenuDisplayView.h"
#include <string>

class MenuDisplayController {
public:
    MenuDisplayController(IPlatform& platform, MenuDisplayView& view,
                          MenuLogic& logic);
    void begin(WindVane& vane);
    void onInput();
    bool updateLiveDisplay(WindVane& vane) const;
    bool checkTimeout() const;
    void showStatusLine(WindVane& vane);
    void setStatusMessage(const char* msg,
                          MenuStatusLevel lvl = MenuStatusLevel::Normal,
                          platform::TimeMs ms = platform::TimeMs{3000});
    void recordCalibration();
    platform::TimeMs lastCalibration() const { return _lastCalibration; }

private:
    void clearExpiredMessage();

    IPlatform& _platform;
    MenuDisplayView& _view;
    MenuLogic& _logic;

    platform::TimeMs _lastActivity;
    platform::TimeMs _lastCalibration;
    std::string _statusMsg;
    MenuStatusLevel _statusLevel;
    platform::TimeMs _msgExpiry;
};
