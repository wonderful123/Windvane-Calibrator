#include "ClusterManager.h"
#include <string>

void ClusterManager::clear() {
    _clusters.clear();
    _anomalyCount = 0;
}

bool ClusterManager::addOrUpdate(float reading, float threshold) {
    for (auto &c : _clusters) {
        if (std::fabs(reading - c.mean) < threshold) {
            c.mean = (c.mean * c.count + reading) / (c.count + 1);
            c.min = std::min(c.min, reading);
            c.max = std::max(c.max, reading);
            ++c.count;
            return false;
        }
    }
    _clusters.push_back({reading, reading, reading, 1});
    return true;
}

void ClusterManager::mergeAndPrune(float mergeThreshold, int minCount) {
    if (_clusters.empty())
        return;
    std::sort(_clusters.begin(), _clusters.end(), [](const ClusterData &a, const ClusterData &b) { return a.mean < b.mean; });
    std::vector<ClusterData> merged;
    size_t i = 0;
    while (i < _clusters.size()) {
        ClusterData cluster = _clusters[i];
        size_t j = i + 1;
        while (j < _clusters.size() && std::fabs(_clusters[j].mean - cluster.mean) < mergeThreshold) {
            float total = cluster.count + _clusters[j].count;
            cluster.mean = (cluster.mean * cluster.count + _clusters[j].mean * _clusters[j].count) / total;
            cluster.min = std::min(cluster.min, _clusters[j].min);
            cluster.max = std::max(cluster.max, _clusters[j].max);
            cluster.count += _clusters[j].count;
            ++j;
        }
        if (cluster.count >= minCount)
            merged.push_back(cluster);
        i = j;
    }
    _clusters = std::move(merged);
}

void ClusterManager::diagnostics(IDiagnostics &diag) const {
    std::string msg = "Anomalies detected: " + std::to_string(_anomalyCount);
    diag.info(msg.c_str());
    for (size_t i = 0; i < _clusters.size(); ++i) {
        float gap = 0;
        if (i + 1 < _clusters.size())
            gap = _clusters[i + 1].mean - _clusters[i].mean;
        msg = "Cluster " + std::to_string(i) +
              ": mean=" + std::to_string(_clusters[i].mean) +
              " min=" + std::to_string(_clusters[i].min) +
              " max=" + std::to_string(_clusters[i].max) +
              " count=" + std::to_string(_clusters[i].count) +
              " gap=" + std::to_string(gap);
        diag.info(msg.c_str());
    }
    if (_clusters.size() > 1) {
        float expectedGap = 1.0f / _clusters.size();
        for (size_t i = 0; i + 1 < _clusters.size(); ++i) {
            float gap = _clusters[i + 1].mean - _clusters[i].mean;
            if (gap < expectedGap * 0.5f) {
                msg = "Warning: clusters " + std::to_string(i) + " and " + std::to_string(i + 1) + " very close";
                diag.warn(msg.c_str());
            }
            if (gap > expectedGap * 1.5f) {
                msg = "Warning: clusters " + std::to_string(i) + " and " + std::to_string(i + 1) + " far apart";
                diag.warn(msg.c_str());
            }
        }
    }
}

