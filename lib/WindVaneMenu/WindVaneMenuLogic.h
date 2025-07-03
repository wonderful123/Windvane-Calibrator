#pragma once
#include "WindVaneStatus.h"
#include <WindVane.h>

class WindVaneMenuLogic {
 public:
  WindVaneStatus queryStatus(WindVane* vane, unsigned long lastCalibration) const;
  const char* statusText(CalibrationManager::CalibrationStatus st) const;
};
