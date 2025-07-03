#pragma once
#include <string>
class WindVaneMenuPresenter;
struct WindVaneStatus;
enum class MenuStatusLevel;

class IPlatform {
public:
    virtual ~IPlatform() = default;
    virtual unsigned long millis() = 0;
    virtual void renderStatusLine(WindVaneMenuPresenter& presenter,
                                  const WindVaneStatus& st,
                                  const char* statusStr,
                                  const std::string& msg,
                                  MenuStatusLevel level) = 0;
};
