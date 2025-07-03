#include <Storage/Settings/FileSettingsStorage.h>
#include <gtest/gtest.h>
#include <fstream>

TEST(FileSettingsStorage, SaveLoadRoundTrip) {
    FileSettingsStorage store("settings.bin");
    SettingsData data{};
    data.spin.bufferSize = 5;
    data.menuState = PersistedMenuState::Diagnostics;
    store.save(data);

    SettingsData out{};
    ASSERT_TRUE(store.load(out).ok());
    EXPECT_EQ(out.spin.bufferSize, 5);
    EXPECT_EQ(static_cast<int>(out.menuState), static_cast<int>(PersistedMenuState::Diagnostics));
    EXPECT_EQ(store.getSchemaVersion(), 1);
}

TEST(FileSettingsStorage, DetectCorruption) {
    FileSettingsStorage store("settings_corrupt.bin");
    SettingsData data{};
    store.save(data);
    std::fstream f("settings_corrupt.bin", std::ios::binary | std::ios::in | std::ios::out);
    f.seekp(sizeof(SettingsStorageHeader));
    char byte;
    f.read(&byte, 1);
    byte ^= 0xFF;
    f.seekp(sizeof(SettingsStorageHeader));
    f.write(&byte, 1);
    f.close();

    SettingsData out{};
    EXPECT_FALSE(store.load(out).ok());
}
