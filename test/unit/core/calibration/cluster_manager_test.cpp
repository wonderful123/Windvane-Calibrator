#include <gtest/gtest.h>
#include "wind_vane/core/calibration/cluster_manager.h"

/**
 * @brief Unit tests for ClusterManager class
 * 
 * Tests the clustering algorithm used in wind vane calibration
 * to ensure proper grouping and interpolation of sensor readings.
 */
class ClusterManagerTest : public ::testing::Test {
protected:
    ClusterManager manager;
    
    void SetUp() override {
        // Setup test data
        manager.clear();
    }
    
    void TearDown() override {
        // Cleanup if needed
    }
};

TEST_F(ClusterManagerTest, EmptyClusterInterpolation) {
    // Test interpolation with no clusters
    float result = manager.interpolate(0.5f);
    EXPECT_FLOAT_EQ(result, 180.0f); // Should return normalized angle
}

TEST_F(ClusterManagerTest, SingleClusterHandling) {
    // Test adding a single cluster
    bool added = manager.addOrUpdate(0.25f, 0.05f);
    EXPECT_TRUE(added);
    EXPECT_EQ(manager.clusters().size(), 1);
}

TEST_F(ClusterManagerTest, ClusterMerging) {
    // Test merging of nearby clusters
    manager.addOrUpdate(0.25f, 0.05f);
    manager.addOrUpdate(0.26f, 0.05f);  // Should merge with first
    
    manager.mergeAndPrune(0.05f, 1);
    EXPECT_EQ(manager.clusters().size(), 1);
}

TEST_F(ClusterManagerTest, InterpolationAccuracy) {
    // Test interpolation between known clusters
    manager.addOrUpdate(0.25f, 0.05f);
    manager.addOrUpdate(0.75f, 0.05f);
    
    float result = manager.interpolate(0.5f);
    EXPECT_NEAR(result, 180.0f, 1.0f); // Should be approximately halfway
}

TEST_F(ClusterManagerTest, WrapAroundInterpolation) {
    // Test interpolation across 0/360 degree boundary
    manager.addOrUpdate(0.1f, 0.05f);   // Near start
    manager.addOrUpdate(0.9f, 0.05f);   // Near end
    
    float result = manager.interpolate(0.05f); // Before first cluster
    EXPECT_GE(result, 0.0f);
    EXPECT_LT(result, 360.0f);
}