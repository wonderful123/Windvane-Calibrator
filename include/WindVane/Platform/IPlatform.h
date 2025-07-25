#pragma once
#include <string>
#include "TimeUtils.h"
class MenuPresenter;
struct WindVaneStatus;
enum class MenuStatusLevel;

class IPlatform {
public:
    virtual ~IPlatform() = default;
    virtual platform::TimeMs millis() const = 0;
    virtual void renderStatusLine(MenuPresenter& presenter,
                                  const WindVaneStatus& st,
                                  const char* statusStr,
                                  const std::string& msg,
                                  MenuStatusLevel level) const = 0;
    virtual bool supportsColor() const = 0;
};
