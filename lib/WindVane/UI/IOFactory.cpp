#include "IOFactory.h"
#include <PlatformFactory.h>

namespace ui {

std::unique_ptr<IUserIO> makeDefaultIO() {
    return platform_factory::makeIO();
}

std::unique_ptr<IOutput> makeDefaultOutput() {
    return platform_factory::makeOutput();
}

void beginPlatformIO(unsigned long baud) {
    platform_factory::beginPlatformIO(baud);
}

} // namespace ui
