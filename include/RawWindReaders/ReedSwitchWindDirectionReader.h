#include "IRawWindReader.h"
#include "IPinReader.h"

class ReedSwitchWindDirectionReader : public IRawWindReader {
public:
    ReedSwitchWindDirectionReader(IPinReader* pinReader);
    double readRawWindDirection() override;

private:
    IPinReader* pinReader_;
};