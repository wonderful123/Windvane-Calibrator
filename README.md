#ESP32 Wind Vane Calibration Project

## Overview

This project aims to provide a robust and flexible calibration mechanism for wind vanes using Arduino. It supports multiple calibration strategies, including user-set directions, compass combination, and spinning the vane multiple times.

The ADC interface used by the library returns values in the range `[0,1]`
representing a full rotation. All calibration and interpolation routines assume
readings are normalized to this range and convert them to degrees in `[0,360)`.

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
calibration. Slowly rotate the vane; the calibrator dynamically clusters
stable readings to determine each switch position. Progress is reported as
"position detected" messages. When all positions are found or you are happy
with the results press `s` to stop and store the calibration. Diagnostics are
printed at the end showing mean, min and max for each detected position.

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
