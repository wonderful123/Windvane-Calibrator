# Basic Wind Vane Example

This example demonstrates basic wind vane functionality with simple wind direction reading and calibration.

## Features

- Simple wind direction measurement
- Basic calibration process
- Serial output for monitoring
- Platform-agnostic design

## Usage

### Arduino IDE

1. Open Arduino IDE
2. Open `BasicWindVane.ino`
3. Select your board (Arduino Uno, ESP32, etc.)
4. Upload the sketch
5. Open Serial Monitor at 115200 baud

### PlatformIO

1. Open the example folder in PlatformIO
2. Select your environment:
   - `esp32dev` for ESP32
   - `uno` for Arduino Uno
   - `native` for desktop simulation
3. Build and upload: `pio run -t upload`
4. Monitor serial output: `pio device monitor`

## Hardware Requirements

- Wind vane sensor (reed switch, potentiometer, etc.)
- Arduino/ESP32 board
- Serial connection for output

## Expected Output

```
WindVane Basic Example
=====================
WindVane initialized successfully
No calibration found, starting calibration...
Calibration started. Rotate the wind vane now.
Calibration progress: 25%
Calibration progress: 50%
Calibration progress: 75%
Calibration progress: 100%
Calibration completed successfully!
Wind Direction: 180°, Raw: 2048, Voltage: 1650 mV
Wind Direction: 90°, Raw: 1024, Voltage: 825 mV
...
```

## Configuration

The example uses default settings:
- Vane Type: Reed Switch
- Calibration Method: Spinning
- Storage: EEPROM (Arduino) / Flash (ESP32)

## Troubleshooting

- **No serial output**: Check baud rate (115200)
- **Calibration fails**: Ensure wind vane is properly connected
- **Build errors**: Make sure WindVane library is installed