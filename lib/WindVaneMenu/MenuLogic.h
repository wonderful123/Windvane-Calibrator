#pragma once
#include "WindVaneStatus.h"
#include <WindVane.h>

#include <Platform/IPlatform.h>

class MenuLogic {
 public:
  WindVaneStatus queryStatus(WindVane* vane, platform::TimeMs lastCalibration,
                             IPlatform& platform) const;
  const char* statusText(CalibrationManager::CalibrationStatus st) const;
};
