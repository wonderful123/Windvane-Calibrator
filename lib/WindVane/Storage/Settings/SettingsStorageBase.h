#pragma once
#include "ISettingsStorage.h"
#include <cstdint>

struct SettingsStorageHeader {
    uint16_t version;
    uint32_t crc;
};

class SettingsStorageBase : public ISettingsStorage {
public:
    int getSchemaVersion() const override { return _schemaVersion; }
protected:
    int _schemaVersion{0};
    static uint32_t crc32(const unsigned char* data, size_t len);
};
