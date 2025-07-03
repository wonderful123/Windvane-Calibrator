#pragma once
#include <IADC.h>
class NullADC : public IADC { public: float read() const override { return 0.0f; } };
