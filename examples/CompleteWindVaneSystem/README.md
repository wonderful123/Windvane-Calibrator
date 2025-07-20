# Complete Wind Vane System Example

This example demonstrates the complete WindVane library with all features including menu system, diagnostics, storage management, and platform abstraction.

## Features

- **Complete System Integration**: All library components working together
- **Interactive Menu System**: Full-featured UI with live display
- **Multiple Calibration Methods**: Spinning, static, manual, automatic
- **Flexible Storage**: EEPROM, Flash, File System support
- **Comprehensive Diagnostics**: Logging, monitoring, and debugging
- **Settings Management**: Configuration persistence
- **Platform Abstraction**: Works on Arduino, ESP32, and Host
- **Real-time Display**: Live wind direction with status
- **Error Handling**: Robust error recovery and validation

## Usage

### Arduino IDE

1. Open Arduino IDE
2. Open `CompleteWindVaneSystem.ino`
3. Select your board (Arduino Uno, ESP32, etc.)
4. Upload the sketch
5. Open Serial Monitor at 115200 baud
6. Use the interactive menu system

### PlatformIO

1. Open the example folder in PlatformIO
2. Select your environment:
   - `esp32dev` for ESP32 (recommended for full features)
   - `uno` for Arduino Uno
   - `native` for desktop simulation
3. Build and upload: `pio run -t upload`
4. Monitor serial output: `pio device monitor`

## System Components

The example demonstrates all library components:

### Core Components
- **WindVane**: Main wind vane class
- **Platform**: Platform abstraction layer
- **ADC**: Analog-to-digital conversion

### UI Components
- **UserIO**: User input handling
- **Output**: Display and logging
- **Menu System**: Interactive interface

### Storage Components
- **CalibrationStorage**: Calibration data persistence
- **SettingsStorage**: Configuration storage
- **SettingsManager**: Settings management

### Diagnostics Components
- **Diagnostics**: Logging and monitoring
- **BufferedDiagnostics**: Buffered logging

## Menu System

The complete menu system provides:

- **Live Display**: Real-time wind direction with status
- **Interactive Calibration**: Guided calibration process
- **Diagnostics**: System diagnostics and logging
- **Settings**: Configuration and maintenance
- **Help**: Documentation and assistance

## Direct Commands

For debugging and testing:

- **H**: Show help
- **S**: Show system status
- **C**: Start calibration
- **D**: Show diagnostics
- **T**: Run system tests
- **R**: Reset system

## Hardware Requirements

- Wind vane sensor (reed switch, potentiometer, etc.)
- Arduino/ESP32 board (ESP32 recommended for full features)
- Serial connection for output

## Expected Output

```
WindVane Complete System
========================
System initialized successfully
Loaded existing calibration
Menu system initialized
System ready!

=== Wind Vane Menu ===
[D] Display direction 
[C] Calibrate        
[G] Diagnostics      
[S] Settings         
[H] Help             
Choose option: 

Live direction - press any key to return
Wind Direction: 180°, Raw: 2048, Voltage: 1650 mV
Status: Calibrated, Points: 360, Valid: Yes
```

## Configuration

The example uses comprehensive configuration:

### Platform Detection
- **ESP32**: Full feature set with Flash storage
- **Arduino**: Basic features with EEPROM storage
- **Host**: Desktop simulation with file storage

### System Components
- **ADC**: Platform-specific ADC configuration
- **Storage**: Platform-appropriate storage backend
- **UI**: Serial interface with menu system
- **Diagnostics**: Comprehensive logging

## Troubleshooting

- **Memory issues**: Use ESP32 for full feature set
- **Menu not responding**: Check serial connection
- **Calibration fails**: Follow guided process
- **Build errors**: Ensure all dependencies installed
- **Storage errors**: Check available memory/storage

## Advanced Features

### Platform Abstraction
- Automatic platform detection
- Platform-specific optimizations
- Cross-platform compatibility

### Error Handling
- Robust error recovery
- Validation and integrity checking
- Graceful degradation

### Performance
- Optimized for real-time operation
- Efficient memory usage
- Fast response times

## Next Steps

After running this example, you can:

1. **Customize Configuration**: Modify settings for your hardware
2. **Add Custom Features**: Extend the library for your needs
3. **Integrate with Other Systems**: Connect to data logging or IoT platforms
4. **Deploy to Production**: Use in real wind monitoring applications