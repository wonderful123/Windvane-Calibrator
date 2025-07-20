# WindVane Library

A professional Arduino library for wind direction sensors with advanced calibration, interactive menu system, and cross-platform support.

## 🚀 Features

### **Core Functionality**
- **Wind Direction Sensing**: Accurate 0-359° wind direction readings
- **Advanced Calibration**: Multiple calibration methods (spinning, static, automatic)
- **Interactive Menu System**: User-friendly interface with real-time display
- **Cross-Platform Support**: Works on Arduino, ESP32, and desktop platforms
- **Flexible Storage**: EEPROM and file-based storage options

### **Professional Architecture**
- **SOLID Principles**: Clean, maintainable code architecture
- **Google C++ Style Guide**: Consistent naming and formatting
- **Dependency Injection**: Loose coupling and testability
- **Platform Abstraction**: Hardware-independent design
- **Comprehensive Diagnostics**: Built-in logging and error handling

### **Easy to Use**
- **Builder Pattern**: Fluent interface for easy setup
- **Self-Contained**: No external dependencies
- **Arduino Library Manager Ready**: Professional library structure
- **Comprehensive Examples**: Multiple example sketches included

## 📦 Installation

### Arduino IDE
1. Download this repository
2. Extract to your Arduino libraries folder
3. Restart Arduino IDE
4. Open `File > Examples > WindVane`

### PlatformIO
```ini
lib_deps = 
    https://github.com/wonderful123/Windvane-Calibrator.git
```

## 🔧 Quick Start

```cpp
#include <WindVane.h>

// Create wind vane with builder pattern
auto windVane = WindVane::Builder()
    .withADC(adc)
    .withStorage(storage)
    .withUserIO(userIO)
    .withDiagnostics(diagnostics)
    .build();

// Get wind direction
float direction = windVane.GetDirection();

// Start calibration
auto result = windVane.StartCalibration();
```

## 📚 Examples

### **BasicWindVane.ino**
Simple wind direction reading with basic calibration.

### **AdvancedCalibration.ino**
Advanced calibration with interactive menu system and multiple methods.

### **CompleteWindVaneSystem.ino**
Full system with menu, diagnostics, storage, and all features.

### **HostSimulation.ino**
Desktop simulation for testing and development.

## 🏗️ Architecture

```
WindVane/
├── Core/           # Main WindVane class
├── Calibration/    # Calibration system
├── Storage/        # Data persistence
├── UI/            # User interface
├── Diagnostics/    # Logging and debugging
├── Platform/      # Platform abstraction
├── Drivers/       # Hardware drivers
├── Menu/          # Interactive menu system
└── Interfaces/    # Abstract interfaces
```

## 🎯 Key Components

### **WindVane Core**
- Main sensor class with dependency injection
- Builder pattern for easy configuration
- Multiple calibration strategies

### **Interactive Menu System**
- Real-time wind direction display
- Interactive calibration process
- System diagnostics and settings
- Help and documentation

### **Advanced Calibration**
- Spinning calibration method
- Static calibration method
- Automatic calibration detection
- Data clustering and validation

### **Flexible Storage**
- EEPROM storage for Arduino
- File storage for desktop
- Settings management
- Data validation and recovery

### **Platform Support**
- Arduino (Uno, Mega, etc.)
- ESP32 (with ADC drivers)
- Desktop (for simulation)

## 🔍 Diagnostics

Built-in diagnostics system provides:
- Real-time sensor readings
- Calibration status
- Error logging
- Performance metrics
- System health monitoring

## 📖 Documentation

- **Examples**: Comprehensive example sketches
- **API Reference**: Complete class documentation
- **Architecture Guide**: Design patterns and principles
- **Platform Guide**: Platform-specific setup

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Follow Google C++ Style Guide
4. Add tests for new features
5. Submit a pull request

## 📄 License

This library is licensed under the MIT License.

## 🆘 Support

- **Issues**: Report bugs and feature requests
- **Examples**: Check the examples folder
- **Documentation**: See inline documentation
- **Community**: Join Arduino community discussions

---

**Professional Wind Direction Sensing Library**  
*Built with SOLID principles and Google C++ Style Guide*
