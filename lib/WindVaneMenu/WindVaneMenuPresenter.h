#pragma once
#include "WindVaneStatus.h"
#include "WindVaneMenuTypes.h"
#include <UI/IIO.h>
#include <string>

class WindVaneMenuPresenter {
 public:
  explicit WindVaneMenuPresenter(IOutput* out) : _out(out) {}
  void renderStatusLineArduino(const WindVaneStatus& st, const char* statusStr,
                               const std::string& msg, MenuStatusLevel level) const;
  void renderStatusLineHost(const WindVaneStatus& st, const char* statusStr,
                            const std::string& msg, MenuStatusLevel level) const;

 private:
  IOutput* _out;
};
