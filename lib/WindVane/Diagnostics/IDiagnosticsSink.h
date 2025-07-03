#pragma once
#include "DiagnosticsEvent.h"

class IDiagnosticsSink {
public:
    virtual ~IDiagnosticsSink() = default;
    virtual void handle(const DiagnosticsEvent& event) = 0;
};
