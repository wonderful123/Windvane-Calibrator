#pragma once
#include "WindVaneStatus.h"
#include "WindVaneMenuTypes.h"
#include <UI/IIO.h>
#include <string>

class WindVaneMenuPresenter {
 public:
  explicit WindVaneMenuPresenter(IOutput* out) : _out(out) {}
  void renderStatusLine(const WindVaneStatus& st, const char* statusStr,
                        const std::string& msg, MenuStatusLevel level,
                        bool color) const;

 private:
  IOutput* _out;
};
