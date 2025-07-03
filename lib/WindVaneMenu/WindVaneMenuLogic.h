#pragma once
#include "WindVaneStatus.h"
#include <WindVane.h>

#include <Platform/IPlatform.h>

class WindVaneMenuLogic {
 public:
  WindVaneStatus queryStatus(WindVane* vane, unsigned long lastCalibration,
                             IPlatform& platform) const;
  const char* statusText(CalibrationManager::CalibrationStatus st) const;
};
