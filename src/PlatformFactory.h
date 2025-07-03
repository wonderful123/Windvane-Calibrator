#pragma once
#include <memory>
#include <Drivers/IADC.h>
#include <Storage/ICalibrationStorage.h>
#include <Storage/Settings/ISettingsStorage.h>
#include <Platform/IPlatform.h>
#include "Config.h"

namespace platform_factory {
std::unique_ptr<IADC> makeADC(const DeviceConfig& cfg);
std::unique_ptr<ICalibrationStorage> makeCalibrationStorage(IPlatform& platform, const DeviceConfig& cfg);
std::unique_ptr<ISettingsStorage> makeSettingsStorage(const DeviceConfig& cfg);
}
