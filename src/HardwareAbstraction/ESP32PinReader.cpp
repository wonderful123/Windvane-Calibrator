#include "HardwareAbstraction/IPinReader.h"

class Esp32PinReader : public IPinReader {
public:
    Esp32PinReader(int pin);
    int readPin() override;

private:
    int pin_;
};

// Implement ESP32-specific code to read pin
Esp32PinReader::Esp32PinReader(int pin) : pin_(pin) {}

int Esp32PinReader::readPin() {
    // ESP32-specific code to read from pin_
    return analogRead(pin_);
}