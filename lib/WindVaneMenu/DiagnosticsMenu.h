#pragma once
#include <WindVane.h>
#include <Diagnostics/IDiagnostics.h>
#include "DiagnosticsView.h"
#include <optional>
#include <functional>

class DiagnosticsMenu {
public:
    DiagnosticsMenu(WindVane& vane,
                    std::optional<std::reference_wrapper<IBufferedDiagnostics>> buffered,
                    DiagnosticsView& view, IDiagnostics& diag);
    void show(platform::TimeMs lastCalibration) const;
    enum class SelfTestStatus { Ok, Failed };
    struct ActionResult { size_t index; bool exit; };
private:
    WindVane& _vane;
    std::optional<std::reference_wrapper<IBufferedDiagnostics>> _buffered;
    DiagnosticsView& _view;
    IDiagnostics& _diag;

    void renderScreen(size_t index, platform::TimeMs lastCalibration) const;
    char readCharBlocking() const;
    ActionResult handleAction(char c, size_t index) const;
    SelfTestStatus selfTest() const;
};
