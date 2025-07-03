#pragma once
#include "WindVaneStatus.h"
#include "MenuTypes.h"
#include <UI/IIO.h>
#include <string>

class MenuPresenter {
 public:
  explicit MenuPresenter(IOutput* out) : _out(out) {}
  void renderStatusLine(const WindVaneStatus& st, const char* statusStr,
                        const std::string& msg, MenuStatusLevel level,
                        bool color) const;

 private:
  IOutput* _out;
};
