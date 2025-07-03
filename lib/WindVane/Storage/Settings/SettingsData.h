#pragma once
#include "../../Calibration/SpinningConfig.h"
#include <WindVaneMenu/MenuTypes.h>

struct SettingsData {
    SpinningConfig spin;
    PersistedMenuState menuState{PersistedMenuState::Main};
};
