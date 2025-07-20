# Advanced Calibration Example

This example demonstrates advanced wind vane features including multiple calibration methods, menu system integration, and comprehensive diagnostics.

## Features

- Multiple calibration methods (spinning, static, manual, automatic)
- Interactive menu system
- Real-time wind direction display
- Diagnostics and logging
- Settings management
- Platform abstraction

## Usage

### Arduino IDE

1. Open Arduino IDE
2. Open `AdvancedCalibration.ino`
3. Select your board (Arduino Uno, ESP32, etc.)
4. Upload the sketch
5. Open Serial Monitor at 115200 baud
6. Use menu commands or direct commands

### PlatformIO

1. Open the example folder in PlatformIO
2. Select your environment:
   - `esp32dev` for ESP32
   - `uno` for Arduino Uno
   - `native` for desktop simulation
3. Build and upload: `pio run -t upload`
4. Monitor serial output: `pio device monitor`

## Menu System

The example includes an interactive menu system:

- **D**: Live wind direction display
- **C**: Start calibration
- **G**: View diagnostics
- **S**: Settings and maintenance
- **H**: Help and documentation

## Direct Commands

You can also use direct commands:

- **1**: Start spinning calibration
- **2**: Start static calibration
- **3**: Start manual calibration
- **4**: Load calibration
- **5**: Save calibration
- **6**: Clear calibration
- **7**: Validate calibration
- **8**: Print calibration data
- **9**: Show menu
- **0**: System information
- **M**: Menu system info

## Hardware Requirements

- Wind vane sensor (reed switch, potentiometer, etc.)
- Arduino/ESP32 board
- Serial connection for output

## Expected Output

```
WindVane Advanced Calibration Example
====================================
WindVane initialized successfully
Menu system initialized
Loaded existing calibration

=== Wind Vane Menu ===
[D] Display direction 
[C] Calibrate        
[G] Diagnostics      
[S] Settings         
[H] Help             
Choose option: 

Live direction - press any key to return
Wind Direction: 180°, Raw: 2048, Voltage: 1650 mV
```

## Configuration

The example uses advanced configuration:
- Vane Type: Potentiometer (ESP32) / Reed Switch (Arduino)
- Calibration Method: Automatic
- Storage: Flash (ESP32) / EEPROM (Arduino)
- Menu System: Interactive serial interface

## Troubleshooting

- **Menu not responding**: Check serial connection and baud rate
- **Calibration fails**: Follow the guided calibration process
- **Build errors**: Ensure all dependencies are installed
- **Memory issues**: Use ESP32 for full feature set