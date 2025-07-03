#pragma once
#include <string>

enum class StorageStatus {
    Ok,
    NotFound,
    IoError,
    CorruptData,
    InvalidFormat,
    InvalidVersion
};

struct StorageResult {
    StorageStatus status{StorageStatus::Ok};
    std::string message{};
    constexpr bool ok() const { return status == StorageStatus::Ok; }
};
