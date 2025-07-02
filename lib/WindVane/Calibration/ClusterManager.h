#pragma once
#include "ClusterData.h"
#include "../Diagnostics/IDiagnostics.h"
#include <algorithm>
#include <cmath>
#include <vector>

class ClusterManager {
public:
    void clear();
    bool addOrUpdate(float reading, float threshold);
    void mergeAndPrune(float mergeThreshold, int minCount);
    void diagnostics(IDiagnostics &diag) const;
    void setClusters(const std::vector<ClusterData>& clusters);
    // reading is expected in the range [0,1]
    float interpolate(float reading) const;
    const std::vector<ClusterData>& clusters() const { return _clusters; }
    int anomalies() const { return _anomalyCount; }
    void recordAnomaly() { ++_anomalyCount; }
private:
    std::vector<ClusterData> _clusters;
    int _anomalyCount{0};
};

