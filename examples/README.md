# WindVane Library Examples

This directory contains comprehensive examples demonstrating all features of the WindVane library, from basic usage to advanced expert features.

## 📚 Example Overview

### 🟢 **SimpleWindVane** - Beginner Level
**Perfect for: First-time users, quick setup, basic understanding**

- **File**: `SimpleWindVane/SimpleWindVane.ino`
- **Complexity**: ⭐ (Beginner)
- **Features**: Basic wind direction reading, compass display
- **Hardware**: Arduino + wind vane sensor
- **Time to Setup**: 5 minutes

**What you'll learn:**
- Basic library setup
- Reading wind direction in degrees
- Converting to compass directions
- Understanding calibration status

### 🟡 **BasicWindVane** - Intermediate Level
**Perfect for: Users who want more control and understanding**

- **File**: `BasicWindVane/BasicWindVane.ino`
- **Complexity**: ⭐⭐ (Intermediate)
- **Features**: Component-based setup, calibration, detailed diagnostics
- **Hardware**: Arduino + wind vane sensor
- **Time to Setup**: 10 minutes

**What you'll learn:**
- Component-based architecture
- ADC configuration and diagnostics
- Automatic calibration process
- Error handling and status monitoring
- Detailed sensor information

### 🟠 **AdvancedCalibration** - Advanced Level
**Perfect for: Users who need advanced calibration and interactive features**

- **File**: `AdvancedCalibration/AdvancedCalibration.ino`
- **Complexity**: ⭐⭐⭐ (Advanced)
- **Features**: Interactive menu system, multiple calibration methods, quality assessment
- **Hardware**: Arduino + wind vane sensor
- **Time to Setup**: 15 minutes

**What you'll learn:**
- Interactive menu system
- Multiple calibration strategies
- Calibration quality assessment
- Advanced error handling
- Real-time progress monitoring
- System diagnostics

### 🔴 **ExpertFeatures** - Expert Level
**Perfect for: Professional users, data analysis, advanced features**

- **File**: `ExpertFeatures/ExpertFeatures.ino`
- **Complexity**: ⭐⭐⭐⭐ (Expert)
- **Features**: Data logging, performance monitoring, multiple storage backends, real-time analysis
- **Hardware**: Arduino/ESP32 + wind vane sensor + optional SD card
- **Time to Setup**: 20 minutes

**What you'll learn:**
- Advanced data logging and analysis
- Performance monitoring and optimization
- Multiple storage backends
- Real-time data processing
- Advanced error handling and recovery
- Custom diagnostic systems
- Memory management
- Data export capabilities

## 🚀 Quick Start Guide

### For Beginners
1. Start with **SimpleWindVane** to understand basic concepts
2. Move to **BasicWindVane** when you want more control
3. Try **AdvancedCalibration** when you need interactive features
4. Use **ExpertFeatures** for professional applications

### For Experienced Users
1. **SimpleWindVane**: Quick reference for basic setup
2. **BasicWindVane**: Component architecture reference
3. **AdvancedCalibration**: Interactive features and quality assessment
4. **ExpertFeatures**: Full professional implementation

## 📋 Hardware Requirements

### Minimum Requirements (SimpleWindVane)
- Arduino board (Uno, Nano, Mega, etc.)
- Wind vane sensor connected to analog pin
- Serial connection for output

### Recommended Requirements (BasicWindVane + AdvancedCalibration)
- Arduino board with 10-bit or better ADC
- Wind vane sensor with good mechanical design
- Serial connection for interactive features

### Expert Requirements (ExpertFeatures)
- Arduino board or ESP32
- High-quality wind vane sensor
- Optional: SD card for file storage
- Optional: External EEPROM for additional storage
- Serial connection for data export

## 🔧 Setup Instructions

### 1. Hardware Setup
```
Wind Vane Sensor → Arduino Analog Pin (A0)
VCC → 5V or 3.3V (check sensor specifications)
GND → GND
```

### 2. Software Setup
1. Install the WindVane library
2. Open the desired example
3. Verify and upload to your board
4. Open Serial Monitor at the specified baud rate

### 3. Calibration (Recommended)
- **SimpleWindVane**: No calibration required for basic operation
- **BasicWindVane**: Automatic calibration during first run
- **AdvancedCalibration**: Interactive calibration with quality assessment
- **ExpertFeatures**: Advanced calibration with multiple quality checks

## 📊 Feature Comparison

| Feature | Simple | Basic | Advanced | Expert |
|---------|--------|-------|----------|--------|
| Basic Reading | ✅ | ✅ | ✅ | ✅ |
| Calibration | ❌ | ✅ | ✅ | ✅ |
| Interactive Menu | ❌ | ❌ | ✅ | ✅ |
| Data Logging | ❌ | ❌ | ❌ | ✅ |
| Performance Monitoring | ❌ | ❌ | ❌ | ✅ |
| Multiple Storage | ❌ | ❌ | ❌ | ✅ |
| Quality Assessment | ❌ | ❌ | ✅ | ✅ |
| Error Handling | Basic | Good | Advanced | Expert |
| Data Export | ❌ | ❌ | ❌ | ✅ |
| Real-time Analysis | ❌ | ❌ | ❌ | ✅ |

## 🎯 Use Cases

### SimpleWindVane
- **Weather stations** - Basic wind direction monitoring
- **Educational projects** - Learning about wind vanes
- **Quick prototypes** - Fast setup and testing
- **Basic automation** - Simple wind-based triggers

### BasicWindVane
- **Home weather stations** - Reliable wind direction with calibration
- **Agricultural monitoring** - Basic wind direction for crop management
- **Hobby projects** - More accurate readings with error handling
- **Educational demonstrations** - Understanding sensor calibration

### AdvancedCalibration
- **Professional weather stations** - High-accuracy wind direction
- **Research projects** - Quality-assured wind measurements
- **Industrial monitoring** - Interactive calibration and diagnostics
- **Educational labs** - Understanding calibration quality

### ExpertFeatures
- **Meteorological stations** - Professional-grade wind monitoring
- **Research institutions** - Data logging and analysis
- **Industrial applications** - Performance monitoring and optimization
- **Data centers** - Multiple storage and backup systems

## 🔍 Troubleshooting

### Common Issues

#### No Readings
- Check sensor connections
- Verify ADC pin configuration
- Ensure proper voltage supply

#### Inaccurate Readings
- Run calibration (BasicWindVane and above)
- Check sensor mechanical alignment
- Verify ADC resolution settings

#### Calibration Fails
- Ensure smooth 360° rotation
- Check for sensor mechanical issues
- Verify sufficient sample collection

#### Memory Issues (ExpertFeatures)
- Reduce log size
- Increase cleanup frequency
- Use external storage

### Debug Tips

#### Serial Output
All examples provide detailed serial output for debugging:
- Sensor readings
- Calibration progress
- Error messages
- System status

#### Diagnostic Features
Advanced examples include comprehensive diagnostics:
- ADC health checks
- Storage status
- Performance metrics
- Memory usage

## 📈 Performance Expectations

### Reading Accuracy
- **Uncalibrated**: ±10-15° typical
- **Basic Calibration**: ±5-10° typical
- **Advanced Calibration**: ±2-5° typical
- **Expert Calibration**: ±1-3° typical

### Update Rates
- **SimpleWindVane**: 1 Hz
- **BasicWindVane**: 0.5 Hz (with diagnostics)
- **AdvancedCalibration**: 0.2 Hz (interactive)
- **ExpertFeatures**: 10 Hz (real-time)

### Memory Usage
- **SimpleWindVane**: < 1KB
- **BasicWindVane**: < 2KB
- **AdvancedCalibration**: < 5KB
- **ExpertFeatures**: 8-16KB (configurable)

## 🔄 Migration Guide

### From SimpleWindVane to BasicWindVane
1. Replace simple setup with component-based setup
2. Add calibration functionality
3. Include error handling
4. Add detailed diagnostics

### From BasicWindVane to AdvancedCalibration
1. Add interactive menu system
2. Implement quality assessment
3. Add multiple calibration methods
4. Include advanced error handling

### From AdvancedCalibration to ExpertFeatures
1. Add data logging capabilities
2. Implement performance monitoring
3. Add multiple storage backends
4. Include real-time analysis
5. Add data export functionality

## 📚 Additional Resources

### Documentation
- [Library Documentation](../README.md)
- [API Reference](../docs/API.md)
- [Troubleshooting Guide](../docs/TROUBLESHOOTING.md)

### Examples by Platform
- **Arduino**: All examples work on Arduino boards
- **ESP32**: ExpertFeatures optimized for ESP32
- **ESP8266**: BasicWindVane and AdvancedCalibration compatible
- **Other Platforms**: Contact library maintainer for porting

### Community
- **GitHub Issues**: Report bugs and request features
- **Discussions**: Ask questions and share projects
- **Contributions**: Submit improvements and new examples

## 🎉 Getting Help

If you encounter issues:

1. **Check the troubleshooting section** above
2. **Review the example code** for your use case
3. **Check the library documentation** for API details
4. **Open a GitHub issue** for bugs or feature requests
5. **Join community discussions** for help and tips

## 📄 License

All examples are provided under the same license as the WindVane library. See the main library README for license details.

---

**Happy Wind Vane Programming! 🌬️**