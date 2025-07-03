#include <Storage/FileCalibrationStorage.h>
#include <Calibration/ClusterData.h>
#include <Storage/CalibrationStorageBase.h>
#include <fstream>
#include <gtest/gtest.h>

TEST(FileCalibrationStorage, SaveLoadRoundTrip) {
    FileCalibrationStorage store("test_calib.bin");
    std::vector<ClusterData> clusters = { {0.1f,0,0,1}, {0.5f,0,0,2} };
    store.save(clusters, 2);

    std::vector<ClusterData> out;
    int version = 0;
    ASSERT_TRUE(store.load(out, version));
    EXPECT_EQ(version, 2);
    EXPECT_EQ(out.size(), clusters.size());
    EXPECT_EQ(store.getSchemaVersion(), 2);
}

TEST(FileCalibrationStorage, DetectCorruption) {
    FileCalibrationStorage store("corrupt.bin");
    std::vector<ClusterData> clusters = { {0.2f,0,0,1} };
    store.save(clusters, 1);
    // corrupt by modifying cluster data
    std::fstream ofs("corrupt.bin", std::ios::binary | std::ios::in | std::ios::out);
    ofs.seekp(sizeof(CalibrationStorageHeader));
    float badFloat;
    ofs.read(reinterpret_cast<char*>(&badFloat), sizeof(float));
    badFloat += 1.0f;
    ofs.seekp(sizeof(CalibrationStorageHeader));
    ofs.write(reinterpret_cast<const char*>(&badFloat), sizeof(float));
    ofs.close();

    std::vector<ClusterData> out;
    int version = 0;
    EXPECT_FALSE(store.load(out, version));
}
