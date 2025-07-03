#include "IOFactory.h"

namespace ui {

std::unique_ptr<IUserIO> makeDefaultIO() {
#ifdef ARDUINO
    return std::make_unique<SerialIOHandler>();
#else
    return std::make_unique<ConsoleIOHandler>();
#endif
}

std::unique_ptr<IOutput> makeDefaultOutput() {
#ifdef ARDUINO
    return std::make_unique<SerialOutput>();
#else
    return std::make_unique<ConsoleOutput>();
#endif
}

void beginPlatformIO(unsigned long baud) {
#ifdef ARDUINO
    Serial.begin(baud);
#else
    (void)baud;
#endif
}

} // namespace ui
