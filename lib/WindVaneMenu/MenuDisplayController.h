#pragma once
#include "MenuPresenter.h"
#include "MenuLogic.h"
#include "WindVaneStatus.h"
#include <Platform/IPlatform.h>
#include <Platform/TimeUtils.h>
#include "MenuDisplayView.h"
#include "MenuState.h"
#include <string>

class MenuDisplayController {
public:
    MenuDisplayController(IPlatform& platform, MenuDisplayView& view,
                          MenuLogic& logic, MenuState& state);
    void begin(WindVane& vane);
    void onInput();
    bool updateLiveDisplay(WindVane& vane) const;
    bool checkTimeout() const;
    void showStatusLine(WindVane& vane);
    void setStatusMessage(const char* msg,
                          MenuStatusLevel lvl = MenuStatusLevel::Normal,
                          platform::TimeMs ms = platform::TimeMs{3000});
    void recordCalibration();
    platform::TimeMs lastCalibration() const { return _state.lastCalibration; }

private:
    void clearExpiredMessage();

    IPlatform& _platform;
    MenuDisplayView& _view;
    MenuLogic& _logic;
    MenuState& _state;
};
