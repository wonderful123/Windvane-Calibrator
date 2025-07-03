#pragma once
#include <WindVane.h>
#include <memory>
#include <WindVaneMenu.h>

#include "Config.h"

class App {
 public:
  App(const DeviceConfig& config);

  void begin();
  void loop();

 private:
  const DeviceConfig& cfg;
  WindVane vane;
  std::unique_ptr<WindVaneMenu> menu;
};
