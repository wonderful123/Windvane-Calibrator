#pragma once
#include "IIOHandler.h"
#include "INumericReader.h"

class IUserIO : public IIOHandler, public INumericReader {
public:
    ~IUserIO() override = default;
};
