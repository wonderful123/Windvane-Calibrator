#pragma once
#include "WindVaneMenuPresenter.h"
#include "WindVaneMenuLogic.h"
#include "WindVaneStatus.h"
#include <Platform/IPlatform.h>
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
                          unsigned long ms = 3000);
    void recordCalibration();
    unsigned long lastCalibration() const { return _lastCalibration; }

private:
    void clearExpiredMessage();

    IPlatform& _platform;
    IUserIO& _io;
    IOutput& _out;
    WindVaneMenuPresenter& _presenter;
    WindVaneMenuLogic& _logic;

    unsigned long _lastActivity;
    unsigned long _lastCalibration;
    std::string _statusMsg;
    MenuStatusLevel _statusLevel;
    unsigned long _msgExpiry;
};
