#pragma once
#include "ConsoleIOHandler.h"
#include "SerialIOHandler.h"
#include "ConsoleOutput.h"
#include "SerialOutput.h"
#include <memory>

namespace ui {
std::unique_ptr<IUserIO> makeDefaultIO();
std::unique_ptr<IOutput> makeDefaultOutput();
void beginPlatformIO(unsigned long baud);
} // namespace ui
