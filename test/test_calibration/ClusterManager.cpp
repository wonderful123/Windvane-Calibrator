#include "../../lib/WindVane/Calibration/ClusterManager.h"
#include <gtest/gtest.h>

TEST(ClusterManagerTest, InterpolateBasic) {
    ClusterManager mgr;
    std::vector<ClusterData> clusters = {
        {0.0f,0,0,1}, {0.25f,0,0,1}, {0.5f,0,0,1}, {0.75f,0,0,1}
    };
    mgr.setClusters(clusters);
    EXPECT_NEAR(mgr.interpolate(0.125f), 45.0f, 1e-2);
    EXPECT_NEAR(mgr.interpolate(0.625f), 225.0f, 1e-2);
    EXPECT_NEAR(mgr.interpolate(0.9f), 324.0f, 1e-2);
}

#if defined(ARDUINO)
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    ::testing::InitGoogleTest();
}

void loop() {
    if (RUN_ALL_TESTS()) ;
    delay(1000);
}
#else
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS()) ;
    return 0;
}
#endif
