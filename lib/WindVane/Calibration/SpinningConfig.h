#pragma once

struct SpinningConfig {
  float threshold = 0.05f;      ///< Minimum delta to consider a new position
  int bufferSize = 5;           ///< Number of samples for noise filtering
  int expectedPositions = 16;   ///< Expected number of detent positions
  int sampleDelayMs = 10;       ///< Delay between samples
  int stallTimeoutSec = 5;      ///< Seconds without new detections before prompt
};
