#pragma once
#include <WindVane.h>
#include <WindVaneMenu.h>

#include "Config.h"  // Add this include

class App {
 public:
  App(const DeviceConfig& config);

  void begin();
  void loop();

 private:
  const DeviceConfig& cfg;
  WindVane vane;
  WindVaneMenu* menu;
};
