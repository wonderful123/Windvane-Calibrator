# WindVane Library

A self-contained C++ library for wind vane calibration and measurement with support for multiple platforms and sensor types.

## Features

- **Multi-Platform Support**: Arduino, ESP32, and Host platforms
- **Multiple Sensor Types**: Reed switch, potentiometer, magnetic, and optical encoders
- **Advanced Calibration**: Spinning, static, automatic, and manual calibration methods
- **Flexible Storage**: EEPROM, Flash, SD card, and file system storage backends
- **SOLID Principles**: Clean architecture with dependency injection and interfaces
- **Comprehensive Testing**: Unit tests and integration tests
- **Google C++ Style Guide**: Consistent code formatting and naming conventions

## Quick Start

### Arduino Example

```cpp
#include <WindVane.h>

// Create WindVane instance
WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::arduino();
auto windVane = builder.setVaneType(WindVane::VaneType::REED_SWITCH)
                       .setCalibrationMethod(WindVane::CalibrationMethod::SPINNING)
                       .build();

void setup() {
  Serial.begin(115200);
  
  if (windVane->begin()) {
    Serial.println("WindVane initialized successfully");
    
    // Load existing calibration
    if (!windVane->loadCalibration()) {
      // Start calibration if none exists
      windVane->startCalibration();
    }
  }
}

void loop() {
  windVane->update();
  
  if (windVane->isCalibrated()) {
    WindVane::WindDirection direction = windVane->getDirection();
    Serial.print("Wind Direction: ");
    Serial.print(direction.getDegrees());
    Serial.println("°");
  }
  
  delay(1000);
}
```

### ESP32 Example

```cpp
#include <WindVane.h>

// Configure for ESP32
WindVane::WindVaneBuilder builder = WindVane::WindVaneBuilder::esp32();
auto windVane = builder.setVaneType(WindVane::VaneType::POTENTIOMETER)
                       .setADCConfig(WindVane::ADCConfig(36, 12, 3300, 8, 50))
                       .setStorageConfig(WindVane::StorageConfig(WindVane::StorageType::FLASH, 0, 2048))
                       .build();
```

## Installation

### Arduino Library Manager

1. Open Arduino IDE
2. Go to Tools > Manage Libraries
3. Search for "WindVane"
4. Click Install

### Manual Installation

1. Download the library
2. Extract to your Arduino libraries folder
3. Restart Arduino IDE

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps = 
    WindVane
```

## Configuration

### Wind Vane Types

- `VaneType::REED_SWITCH`: Reed switch based wind vane
- `VaneType::POTENTIOMETER`: Potentiometer based wind vane
- `VaneType::MAGNETIC`: Magnetic sensor based wind vane
- `VaneType::OPTICAL`: Optical encoder based wind vane

### Calibration Methods

- `CalibrationMethod::SPINNING`: Rotate wind vane 360° continuously
- `CalibrationMethod::STATIC`: Point to known directions
- `CalibrationMethod::AUTOMATIC`: Automatic calibration
- `CalibrationMethod::MANUAL`: Manual calibration with user input

### Storage Types

- `StorageType::EEPROM`: EEPROM storage (Arduino)
- `StorageType::FLASH`: Flash memory storage (ESP32)
- `StorageType::SD_CARD`: SD card storage
- `StorageType::FILE_SYSTEM`: File system storage (Host)

## API Reference

### Main Classes

#### WindVane

The main class for wind vane operations.

```cpp
class WindVane {
public:
    // Constructor with dependency injection
    WindVane(const WindVaneConfig& config,
              std::unique_ptr<IADC> adc,
              std::unique_ptr<ICalibrationStorage> storage,
              std::unique_ptr<IUserIO> io,
              std::unique_ptr<IDiagnostics> diagnostics);
    
    // Core methods
    bool begin();
    void end();
    void update();
    
    // Measurement methods
    WindDirection getDirection() const;
    WindMeasurement getMeasurement() const;
    uint16_t getRawValue() const;
    uint16_t getVoltage() const;
    
    // Calibration methods
    bool isCalibrated() const;
    bool startCalibration(CalibrationMethod method = CalibrationMethod::SPINNING);
    bool stopCalibration();
    bool isCalibrating() const;
    uint8_t getCalibrationProgress() const;
    
    // Storage methods
    bool loadCalibration();
    bool saveCalibration();
    bool clearCalibration();
    CalibrationData getCalibrationData() const;
};
```

#### WindVaneBuilder

Builder pattern for creating WindVane instances.

```cpp
class WindVaneBuilder {
public:
    // Configuration methods
    WindVaneBuilder& setVaneType(VaneType type);
    WindVaneBuilder& setCalibrationMethod(CalibrationMethod method);
    WindVaneBuilder& setStorageType(StorageType type);
    WindVaneBuilder& setPlatformType(PlatformType type);
    WindVaneBuilder& setADCConfig(const ADCConfig& config);
    WindVaneBuilder& setStorageConfig(const StorageConfig& config);
    
    // Factory methods
    static WindVaneBuilder arduino();
    static WindVaneBuilder esp32();
    static WindVaneBuilder host();
    
    // Build method
    std::unique_ptr<WindVane> build();
};
```

### Data Types

#### WindDirection

```cpp
struct WindDirection {
    uint16_t degrees;
    
    uint16_t getDegrees() const;
    double getRadians() const;
};
```

#### WindSpeed

```cpp
struct WindSpeed {
    float mps;
    
    float getMPS() const;
    float getKPH() const;
    float getMPH() const;
    float getKnots() const;
};
```

#### WindMeasurement

```cpp
struct WindMeasurement {
    WindDirection direction;
    WindSpeed speed;
    TimeMs timestamp;
    bool isValid;
};
```

## Examples

### Basic Wind Vane

Simple example for reading wind direction.

```cpp
// examples/BasicWindVane/BasicWindVane.ino
```

### Advanced Calibration

Advanced example with multiple calibration methods.

```cpp
// examples/AdvancedCalibration/AdvancedCalibration.ino
```

### Host Simulation

Desktop simulation with file-based storage.

```cpp
// examples/HostSimulation/HostSimulation.cpp
```

## Platform Support

### Arduino

- **ADC**: Built-in ADC
- **Storage**: EEPROM
- **UI**: Serial communication
- **Time**: `millis()` function

### ESP32

- **ADC**: ESP32 ADC with high resolution
- **Storage**: Flash memory or SD card
- **UI**: Serial communication
- **Time**: `millis()` function

### Host (Desktop)

- **ADC**: Simulated ADC values
- **Storage**: File system
- **UI**: Console I/O
- **Time**: `std::chrono`

## Architecture

The library follows SOLID principles with clean separation of concerns:

- **Interfaces**: Abstract interfaces for all dependencies
- **Dependency Injection**: All dependencies injected through constructor
- **Builder Pattern**: Fluent interface for configuration
- **Factory Pattern**: Platform-specific implementations
- **Strategy Pattern**: Different calibration and storage strategies

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

## License

MIT License - see LICENSE file for details.

## Version History

- **1.0.0**: Initial release with self-contained library structure
- Support for Arduino, ESP32, and Host platforms
- Multiple calibration methods
- Flexible storage backends
- Comprehensive testing framework
