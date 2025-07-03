#pragma once
#include "MenuPresenter.h"
#include "WindVaneStatus.h"
#include <UI/IIO.h>
#include <Platform/IPlatform.h>
#include <WindVane.h>
#include <string>
#include "MenuState.h"

class MenuDisplayView {
public:
    MenuDisplayView(IPlatform& platform, IUserIO& io, IOutput& out, MenuPresenter& presenter);
    bool updateLiveDisplay(WindVane& vane, MenuState& state) const;
    void renderStatus(const WindVaneStatus& st, const char* statusStr,
                      const std::string& msg, MenuStatusLevel level) const;
private:
    IPlatform& _platform;
    IUserIO& _io;
    IOutput& _out;
    MenuPresenter& _presenter;
};
