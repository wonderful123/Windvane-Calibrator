#include "App.h"
#include "Config.h"

DeviceConfig deviceCfg = defaultDeviceConfig();
App app(deviceCfg);

void setup() { app.begin(); }
void loop() { app.loop(); }
