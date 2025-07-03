#pragma once
#include "WindVaneMenuPresenter.h"
#include "WindVaneMenuLogic.h"
#include "WindVaneStatus.h"
#include <Platform/IPlatform.h>
#include <Platform/TimeUtils.h>
#include <UI/IIO.h>
#include <string>

class WindVaneMenuDisplayController {
public:
    WindVaneMenuDisplayController(IPlatform& platform, IUserIO& io, IOutput& out,
                                  WindVaneMenuPresenter& presenter,
                                  WindVaneMenuLogic& logic);
    void begin(WindVane& vane);
    void onInput();
    bool updateLiveDisplay(WindVane& vane);
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
    IUserIO& _io;
    IOutput& _out;
    WindVaneMenuPresenter& _presenter;
    WindVaneMenuLogic& _logic;

    platform::TimeMs _lastActivity;
    platform::TimeMs _lastCalibration;
    std::string _statusMsg;
    MenuStatusLevel _statusLevel;
    platform::TimeMs _msgExpiry;
};
