# ESP32 Wind Vane Calibration Project

## Overview

This project aims to provide a robust and flexible calibration mechanism for wind vanes using Arduino. It supports multiple calibration strategies, including user-set directions, compass combination, and spinning the vane multiple times.

---

## Table of Contents

1. [Installation](#installation)
2. [Usage](#usage)
3. [Calibration Strategies](#calibration-strategies)
4. [Testing](#testing)
5. [Contributing](#contributing)

---

## Installation

1. Clone the repository
   ```
   git clone https://github.com/yourusername/arduino-wind-vane-calibration.git
   ```
2. Open the project in your preferred Arduino IDE
3. Install any required libraries and dependencies

---

## Usage

Upload the code to your Arduino board and open the serial monitor. After
aligning the vane to your forward reference press any key to begin
calibration. Rotate the vane slowly; readings are clustered into unique
positions using an adaptive threshold with debouncing and anomaly
detection. Progress is displayed as a certainty percentage that increases
as more rotations confirm the map. Press `s` to stop once you are satisfied
with the certainty.

---

## Calibration Strategies

This project supports multiple calibration strategies. The `CalibrationStrategy` interface allows you to easily switch between different methods.

### User-Set Directions

Manually set the vane to specific directions and record the sensor readings.

### Compass Combination

Combine the vane readings with a digital compass for automatic calibration.

### Spinning Method

Spin the wind vane multiple times and record sensor data to build a calibration model.

---

## Testing

Unit and integration tests are available for each calibration strategy. Run the tests to ensure the system is working as expected.

---

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request
