#pragma once
#include "IIO.h"
#include <memory>

namespace ui {
std::unique_ptr<IUserIO> makeDefaultIO();
std::unique_ptr<IOutput> makeDefaultOutput();
void beginPlatformIO(unsigned long baud);
} // namespace ui
