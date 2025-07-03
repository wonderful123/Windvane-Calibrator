#include "CalibrationStorageBase.h"

uint32_t CalibrationStorageBase::crc32(const unsigned char* data, size_t len) {
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320u;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFFu;
}

uint32_t CalibrationStorageBase::crc32(const std::vector<ClusterData>& clusters) {
    return crc32(reinterpret_cast<const unsigned char*>(clusters.data()),
                 clusters.size() * sizeof(ClusterData));
}
