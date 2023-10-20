#include "RawWindReaders/ReedSwitchWindDirectionReader.h"

ReedSwitchWindDirectionReader::ReedSwitchWindDirectionReader(IPinReader* pinReader)
    : pinReader_(pinReader) {}

double ReedSwitchWindDirectionReader::readRawWindDirection() {
    int rawValue = pinReader_->readPin();
    // Convert rawValue to wind direction in degrees
    // ...
    return 0.0; // Placeholder
}