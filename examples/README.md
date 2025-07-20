# WindVane Library Examples

This directory contains comprehensive examples demonstrating the WindVane library features. All examples are compatible with both **Arduino IDE** and **PlatformIO**.

## Quick Start

### Arduino IDE
1. Install the WindVane library via Library Manager
2. Open any `.ino` file in the examples
3. Select your board and upload
4. Open Serial Monitor at 115200 baud

### PlatformIO
1. Open any example folder in PlatformIO
2. Select your environment (esp32dev, uno, native)
3. Build and upload: `pio run -t upload`
4. Monitor output: `pio device monitor`

## Examples Overview

### 1. BasicWindVane
**Difficulty**: Beginner  
**Hardware**: Any Arduino/ESP32 + wind vane sensor  
**Features**: Simple wind direction reading and basic calibration

**Best for**: Getting started, understanding basic concepts

### 2. AdvancedCalibration  
**Difficulty**: Intermediate  
**Hardware**: ESP32 recommended (for full features)  
**Features**: Multiple calibration methods, menu system, diagnostics

**Best for**: Learning advanced features, testing different calibration methods

### 3. CompleteWindVaneSystem
**Difficulty**: Advanced  
**Hardware**: ESP32 recommended (for full features)  
**Features**: Complete system with all components, interactive menu, comprehensive diagnostics

**Best for**: Production applications, understanding full system architecture

### 4. HostSimulation
**Difficulty**: Beginner  
**Hardware**: None required (desktop simulation)  
**Features**: Desktop simulation with realistic wind patterns

**Best for**: Development, testing, learning without hardware

## Platform Support

| Example | Arduino Uno | ESP32 | Desktop |
|---------|-------------|-------|---------|
| BasicWindVane | ✅ | ✅ | ✅ |
| AdvancedCalibration | ⚠️ Limited | ✅ | ✅ |
| CompleteWindVaneSystem | ⚠️ Limited | ✅ | ✅ |
| HostSimulation | ❌ | ❌ | ✅ |

**Legend**: ✅ Full support, ⚠️ Limited features, ❌ Not supported

## Hardware Requirements

### Basic Requirements
- **Wind vane sensor**: Reed switch, potentiometer, magnetic, or optical
- **Microcontroller**: Arduino Uno, ESP32, or compatible
- **Serial connection**: For output and debugging

### Recommended Setup
- **ESP32**: For full feature set and best performance
- **Potentiometer wind vane**: For most accurate readings
- **External power**: For stable operation

## Getting Started

### Step 1: Choose Your Example

**For beginners**: Start with `BasicWindVane`  
**For learning**: Try `AdvancedCalibration`  
**For production**: Use `CompleteWindVaneSystem`  
**For development**: Use `HostSimulation`

### Step 2: Set Up Your Environment

#### Arduino IDE
```bash
# Install library via Library Manager
# Search for "WindVane" and install
```

#### PlatformIO
```bash
# Add to platformio.ini
lib_deps = 
    WindVane
```

### Step 3: Configure for Your Hardware

#### ESP32 Configuration
```cpp
WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::esp32();
builder.setVaneType(WindVane::VaneType::POTENTIOMETER)
       .setADCConfig(WindVane::ADCConfig(36, 12, 3300, 8, 50));
```

#### Arduino Configuration
```cpp
WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::arduino();
builder.setVaneType(WindVane::VaneType::REED_SWITCH);
```

### Step 4: Upload and Test

1. Upload the sketch to your board
2. Open Serial Monitor at 115200 baud
3. Follow the on-screen instructions
4. Test calibration and wind direction reading

## Troubleshooting

### Common Issues

#### Build Errors
- **"WindVane not found"**: Install the library via Library Manager
- **"C++17 not supported"**: Update Arduino IDE or use PlatformIO
- **"Platform not supported"**: Check platform configuration

#### Runtime Errors
- **"Calibration failed"**: Check sensor connections
- **"No serial output"**: Verify baud rate (115200)
- **"Memory issues"**: Use ESP32 for full features

#### Hardware Issues
- **"ADC errors"**: Check sensor wiring and power
- **"Unstable readings"**: Ensure stable power supply
- **"Wrong direction"**: Calibrate the wind vane

### Debugging Tips

1. **Use Serial Monitor**: All examples provide detailed output
2. **Check Connections**: Verify sensor wiring
3. **Test ADC**: Use basic ADC reading first
4. **Calibrate Properly**: Follow calibration instructions
5. **Check Power**: Ensure stable voltage supply

## Advanced Usage

### Customizing Examples

#### Modify ADC Configuration
```cpp
builder.setADCConfig(WindVane::ADCConfig(
    pin,        // ADC pin number
    resolution, // ADC resolution (10, 12 bits)
    voltage,    // Reference voltage (mV)
    samples,    // Number of samples to average
    delay       // Delay between samples (ms)
));
```

#### Change Storage Backend
```cpp
builder.setStorageConfig(WindVane::StorageConfig(
    WindVane::StorageType::FLASH,  // Storage type
    0,                              // Start address
    2048                            // Size in bytes
));
```

#### Custom Calibration
```cpp
windVane->startCalibration(WindVane::CalibrationMethod::MANUAL);
// Follow prompts for manual calibration
```

### Integration Examples

#### Data Logging
```cpp
// Log wind direction to SD card
if (windVane->isCalibrated()) {
    WindVane::WindDirection direction = windVane->getDirection();
    logToSD(direction.getDegrees());
}
```

#### Web Interface
```cpp
// Send data to web server
if (windVane->isCalibrated()) {
    WindVane::WindDirection direction = windVane->getDirection();
    sendToServer(direction.getDegrees());
}
```

#### IoT Integration
```cpp
// Send to IoT platform
if (windVane->isCalibrated()) {
    WindVane::WindDirection direction = windVane->getDirection();
    publishToMQTT(direction.getDegrees());
}
```

## Example Outputs

### BasicWindVane
```
WindVane Basic Example
=====================
WindVane initialized successfully
No calibration found, starting calibration...
Calibration started. Rotate the wind vane now.
Calibration progress: 100%
Calibration completed successfully!
Wind Direction: 180°, Raw: 2048, Voltage: 1650 mV
```

### AdvancedCalibration
```
WindVane Advanced Calibration Example
=====================================
WindVane initialized successfully
Menu system initialized

=== Wind Vane Menu ===
[D] Display direction 
[C] Calibrate        
[G] Diagnostics      
[S] Settings         
[H] Help             
Choose option: 
```

### CompleteWindVaneSystem
```
WindVane Complete System
========================
System initialized successfully
Loaded existing calibration
Menu system initialized
System ready!

Live direction - press any key to return
Wind Direction: 180°, Raw: 2048, Voltage: 1650 mV
Status: Calibrated, Points: 360, Valid: Yes
```

### HostSimulation
```
WindVane Host Simulation Example
=================================
WindVane initialized successfully
No calibration found, starting automatic calibration...
Calibration progress: 100%
Calibration completed successfully!

Simulation Step 1:
  Simulated Angle: 45.2°
  Measured Direction: 45°
  Raw ADC Value: 512
  Voltage: 413 mV
  Calibrated: Yes
```

## Next Steps

After running the examples:

1. **Customize for Your Hardware**: Modify ADC and storage configuration
2. **Add Your Features**: Extend the examples for your specific needs
3. **Integrate with Other Systems**: Connect to data logging, IoT, or web platforms
4. **Deploy to Production**: Use the complete system for real applications

## Support

For issues and questions:

1. **Check the troubleshooting section** above
2. **Review the example READMEs** for specific guidance
3. **Test with HostSimulation** first to isolate hardware issues
4. **Use the menu system** for diagnostics and settings

## Contributing

To contribute examples:

1. Follow the existing structure
2. Include both Arduino and PlatformIO support
3. Add comprehensive README documentation
4. Test on multiple platforms
5. Include troubleshooting guidance