# Standard C++ File Naming Conventions

## Overview
This document outlines the implementation of standard C++ naming conventions for the Wind Vane Calibrator embedded project, using PascalCase and .cpp extensions where filenames directly reflect class names.

## Standard C++ Naming Conventions

### File Naming Rules
1. **PascalCase** for all filenames
2. **Headers end in `.h`**
3. **Source files end in `.cpp`**
4. **Filename matches class name** exactly
5. **Test files** use `Test` suffix: `ClassNameTest.cpp`
6. **Interfaces** use `I` prefix: `IClassName.h`
7. **Abstract classes** use `Abstract` prefix: `AbstractClassName.h`

### Class and Interface Naming
- **Classes**: `PascalCase` - `WindVane`, `CalibrationManager`
- **Interfaces**: `I` prefix - `IAnalogDigitalConverter`, `IUserInterface`
- **Abstract classes**: `Abstract` prefix - `AbstractCalibrationStrategy`
- **Enums**: `PascalCase` - `CalibrationStatus`, `StorageResult`
- **Methods**: `PascalCase` - `GetDirection()`, `StartCalibration()`
- **Member variables**: `camelCase` with prefix - `m_adc`, `m_storage`

## Current → Standard C++ Compliant Mapping

### Core Interface Files
| Current File | Standard C++ Compliant | Class/Interface Name | Reason |
|--------------|------------------------|---------------------|---------|
| `IADC.h` | `IAnalogDigitalConverter.h` | `IAnalogDigitalConverter` | Clear interface name |
| `IIO.h` | `IUserInterface.h` | `IUserInterface` | Descriptive interface |
| `ICalibrationStorage.h` | `ICalibrationStorage.h` | `ICalibrationStorage` | Already compliant |
| `IDiagnostics.h` | `IDiagnostics.h` | `IDiagnostics` | Already compliant |
| `IPlatform.h` | `IPlatform.h` | `IPlatform` | Already compliant |

### Implementation Files  
| Current File | Standard C++ Compliant | Class Name | Reason |
|--------------|------------------------|------------|---------|
| `ESP32ADC.cpp/.h` | `Esp32AnalogConverter.cpp/.h` | `Esp32AnalogConverter` | Clear implementation name |
| `NullADC.h` | `MockAnalogConverter.h` | `MockAnalogConverter` | Purpose-driven naming |
| `WindVane.cpp/.h` | `WindVane.cpp/.h` | `WindVane` | Already compliant |
| `CalibrationManager.cpp/.h` | `CalibrationManager.cpp/.h` | `CalibrationManager` | Already compliant |
| `ClusterManager.cpp/.h` | `ClusterManager.cpp/.h` | `ClusterManager` | Already compliant |

### Storage Components
| Current File | Standard C++ Compliant | Class Name | Reason |
|--------------|------------------------|------------|---------|
| `EEPROMCalibrationStorage.cpp/.h` | `EepromCalibrationStorage.cpp/.h` | `EepromCalibrationStorage` | Standard acronym casing |
| `FileCalibrationStorage.cpp/.h` | `FileCalibrationStorage.cpp/.h` | `FileCalibrationStorage` | Already compliant |
| `SettingsManager.cpp/.h` | `SettingsManager.cpp/.h` | `SettingsManager` | Already compliant |

### UI Components
| Current File | Standard C++ Compliant | Class Name | Reason |
|--------------|------------------------|------------|---------|
| `WindVaneMenu.cpp/.h` | `MainMenu.cpp/.h` | `MainMenu` | Simplified naming |
| `DiagnosticsMenu.cpp/.h` | `DiagnosticsMenu.cpp/.h` | `DiagnosticsMenu` | Already compliant |
| `SettingsMenu.cpp/.h` | `SettingsMenu.cpp/.h` | `SettingsMenu` | Already compliant |
| `MenuDisplayController.cpp/.h` | `MenuDisplayController.cpp/.h` | `MenuDisplayController` | Already compliant |

### Calibration Components
| Current File | Standard C++ Compliant | Class Name | Reason |
|--------------|------------------------|------------|---------|
| `SpinningMethod.cpp/.h` | `SpinningCalibrationStrategy.cpp/.h` | `SpinningCalibrationStrategy` | Clear strategy pattern |
| `StrategyFactory.cpp/.h` | `CalibrationStrategyFactory.cpp/.h` | `CalibrationStrategyFactory` | Descriptive factory |

### Test Files
| Current File | Standard C++ Compliant | Reason |
|--------------|------------------------|---------|
| `test_calibration/ClusterManager.cpp` | `ClusterManagerTest.cpp` | Standard test naming |
| `test_io/IOHandlers.cpp` | `UserInterfaceTest.cpp` | Clear test purpose |
| `test_storage/FileCalibrationStorage.cpp` | `FileCalibrationStorageTest.cpp` | Matches class name |
| `test_hardwareinterface/ESP32ADC.cpp` | `Esp32AnalogConverterTest.cpp` | Matches implementation |

## Directory Structure (Standard C++ Compliant)

```
lib/
├── Core/
│   ├── WindVane.h
│   ├── WindVane.cpp
│   ├── Calibration/
│   │   ├── CalibrationManager.h
│   │   ├── CalibrationManager.cpp
│   │   ├── ClusterManager.h
│   │   ├── ClusterManager.cpp
│   │   └── Strategies/
│   │       ├── ICalibrationStrategy.h
│   │       ├── SpinningCalibrationStrategy.h
│   │       ├── SpinningCalibrationStrategy.cpp
│   │       ├── CalibrationStrategyFactory.h
│   │       └── CalibrationStrategyFactory.cpp
│   └── Measurement/
│       ├── DirectionCalculator.h
│       └── DirectionCalculator.cpp
├── Interfaces/
│   ├── IAnalogDigitalConverter.h
│   ├── ICalibrationStorage.h
│   ├── IUserInterface.h
│   ├── IDiagnostics.h
│   └── IPlatform.h
├── Hardware/
│   ├── Esp32/
│   │   ├── Esp32AnalogConverter.h
│   │   ├── Esp32AnalogConverter.cpp
│   │   ├── Esp32SerialInterface.h
│   │   └── Esp32SerialInterface.cpp
│   └── Mock/
│       ├── MockAnalogConverter.h
│       ├── MockAnalogConverter.cpp
│       ├── MockUserInterface.h
│       └── MockUserInterface.cpp
├── Storage/
│   ├── EepromCalibrationStorage.h
│   ├── EepromCalibrationStorage.cpp
│   ├── FileCalibrationStorage.h
│   ├── FileCalibrationStorage.cpp
│   ├── SettingsManager.h
│   └── SettingsManager.cpp
├── UI/
│   ├── MainMenu.h
│   ├── MainMenu.cpp
│   ├── DiagnosticsMenu.h
│   ├── DiagnosticsMenu.cpp
│   ├── SettingsMenu.h
│   ├── SettingsMenu.cpp
│   ├── MenuDisplayController.h
│   └── MenuDisplayController.cpp
└── Services/
    ├── DependencyContainer.h
    ├── DependencyContainer.cpp
    ├── PlatformFactory.h
    └── PlatformFactory.cpp

src/
├── Application.h
├── Application.cpp
├── main.cpp
└── Config.h

test/
├── Unit/
│   ├── Core/
│   │   ├── WindVaneTest.cpp
│   │   └── Calibration/
│   │       ├── ClusterManagerTest.cpp
│   │       ├── CalibrationManagerTest.cpp
│   │       └── SpinningCalibrationStrategyTest.cpp
│   ├── Storage/
│   │   ├── FileCalibrationStorageTest.cpp
│   │   ├── EepromCalibrationStorageTest.cpp
│   │   └── SettingsManagerTest.cpp
│   ├── Hardware/
│   │   ├── Esp32AnalogConverterTest.cpp
│   │   └── MockAnalogConverterTest.cpp
│   └── UI/
│       ├── MainMenuTest.cpp
│       ├── DiagnosticsMenuTest.cpp
│       └── MenuDisplayControllerTest.cpp
├── Integration/
│   ├── CalibrationWorkflowTest.cpp
│   ├── MenuNavigationTest.cpp
│   └── HardwareIntegrationTest.cpp
└── Fixtures/
    ├── TestData.h
    ├── TestData.cpp
    ├── MockImplementations.h
    └── MockImplementations.cpp
```

## Implementation Examples

### Interface Definition (Standard C++)
```cpp
// lib/Interfaces/IAnalogDigitalConverter.h
#ifndef IANALOGDIGITALCONVERTER_H
#define IANALOGDIGITALCONVERTER_H

/**
 * @brief Interface for analog-to-digital converter hardware abstraction
 */
class IAnalogDigitalConverter {
public:
    virtual ~IAnalogDigitalConverter() = default;
    
    /**
     * @brief Read normalized analog value from the sensor
     * @return Normalized value between 0.0 and 1.0
     */
    virtual float ReadNormalized() const = 0;
    
    /**
     * @brief Read raw analog value from the sensor
     * @return Raw ADC value (platform-specific range)
     */
    virtual int ReadRaw() const = 0;
    
    /**
     * @brief Check if the ADC is available and functioning
     * @return true if ADC is ready for reading
     */
    virtual bool IsAvailable() const = 0;
    
    /**
     * @brief Get the resolution of this ADC in bits
     * @return ADC resolution (e.g., 12 for ESP32, 10 for Arduino Uno)
     */
    virtual int GetResolutionBits() const = 0;
};

#endif // IANALOGDIGITALCONVERTER_H
```

### Class Implementation (Standard C++)
```cpp
// lib/Core/WindVane.h
#ifndef WINDVANE_H
#define WINDVANE_H

#include <memory>
#include "../Interfaces/IAnalogDigitalConverter.h"
#include "../Interfaces/ICalibrationStorage.h"
#include "../Interfaces/IDiagnostics.h"
#include "../Interfaces/IUserInterface.h"

// Forward declarations
class CalibrationManager;
struct CalibrationResult;

/**
 * @brief Main wind vane sensor class
 */
class WindVane {
public:
    /**
     * @brief Configuration structure for WindVane dependencies
     */
    struct Config {
        IAnalogDigitalConverter& adc;
        ICalibrationStorage* storage = nullptr;
        IUserInterface& userInterface;
        IDiagnostics& diagnostics;
    };

    /**
     * @brief Construct wind vane with required dependencies
     * @param config Configuration containing all required dependencies
     */
    explicit WindVane(const Config& config);
    
    // Rule of Five
    ~WindVane() = default;
    WindVane(const WindVane&) = delete;
    WindVane& operator=(const WindVane&) = delete;
    WindVane(WindVane&&) = default;
    WindVane& operator=(WindVane&&) = default;

    /**
     * @brief Get current wind direction in degrees
     * @return Wind direction (0-359 degrees, 0 = North)
     */
    float GetDirection() const;

    /**
     * @brief Get raw sensor reading without calibration
     * @return Raw sensor value (0.0-1.0)
     */
    float GetRawReading() const;

    /**
     * @brief Start calibration process
     * @return Calibration result with success status and details
     */
    CalibrationResult StartCalibration();

    /**
     * @brief Check if sensor is calibrated and ready
     * @return true if calibration is valid
     */
    bool IsCalibrated() const;

    /**
     * @brief Clear stored calibration data
     * @return true if clearing was successful
     */
    bool ClearCalibration();

private:
    IAnalogDigitalConverter& m_adc;
    ICalibrationStorage* m_storage;
    IUserInterface& m_userInterface;
    IDiagnostics& m_diagnostics;
    
    std::unique_ptr<CalibrationManager> m_calibrationManager;
};

#endif // WINDVANE_H
```

### Test Implementation (Standard C++)
```cpp
// test/Unit/Core/WindVaneTest.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../lib/Core/WindVane.h"
#include "../../../lib/Interfaces/IAnalogDigitalConverter.h"

class MockAnalogDigitalConverter : public IAnalogDigitalConverter {
public:
    MOCK_METHOD(float, ReadNormalized, (), (const, override));
    MOCK_METHOD(int, ReadRaw, (), (const, override));
    MOCK_METHOD(bool, IsAvailable, (), (const, override));
    MOCK_METHOD(int, GetResolutionBits, (), (const, override));
};

class WindVaneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup default mock behavior
        ON_CALL(m_mockAdc, IsAvailable())
            .WillByDefault(::testing::Return(true));
        ON_CALL(m_mockAdc, ReadNormalized())
            .WillByDefault(::testing::Return(0.5f));
    }

    MockAnalogDigitalConverter m_mockAdc;
    // ... other mocks
};

TEST_F(WindVaneTest, GetRawReading_ReturnsAdcValue) {
    const float expectedReading = 0.75f;
    
    EXPECT_CALL(m_mockAdc, ReadNormalized())
        .WillOnce(::testing::Return(expectedReading));
    
    // ... test implementation
}
```

## Migration Scripts

### Script 1: Rename to Standard C++ Conventions
```bash
#!/bin/bash
# migrate_to_standard_cpp.sh

# Function to rename a file pair
rename_to_standard_cpp() {
    local old_name=$1
    local new_name=$2
    local directory=$3
    
    if [ -f "${directory}/${old_name}.h" ]; then
        mv "${directory}/${old_name}.h" "${directory}/${new_name}.h"
        echo "Renamed ${old_name}.h → ${new_name}.h"
    fi
    
    if [ -f "${directory}/${old_name}.cpp" ]; then
        mv "${directory}/${old_name}.cpp" "${directory}/${new_name}.cpp"
        echo "Renamed ${old_name}.cpp → ${new_name}.cpp"
    fi
}

# Rename interface files
echo "Renaming interface files..."
rename_to_standard_cpp "IADC" "IAnalogDigitalConverter" "lib/Interfaces"
rename_to_standard_cpp "IIO" "IUserInterface" "lib/Interfaces"

# Rename implementation files
echo "Renaming implementation files..."
rename_to_standard_cpp "ESP32ADC" "Esp32AnalogConverter" "lib/Hardware/Esp32"
rename_to_standard_cpp "NullADC" "MockAnalogConverter" "lib/Hardware/Mock"

# Rename menu files
echo "Renaming UI files..."
rename_to_standard_cpp "WindVaneMenu" "MainMenu" "lib/UI"

echo "Standard C++ naming migration complete!"
```

### Script 2: Update Include Statements
```bash
#!/bin/bash
# update_includes_standard_cpp.sh

update_includes_standard_cpp() {
    local file=$1
    
    # Update interface includes
    sed -i 's|#include <IADC.h>|#include "../Interfaces/IAnalogDigitalConverter.h"|g' "$file"
    sed -i 's|#include <IIO.h>|#include "../Interfaces/IUserInterface.h"|g' "$file"
    
    # Update implementation includes
    sed -i 's|#include "ESP32ADC.h"|#include "Esp32AnalogConverter.h"|g' "$file"
    sed -i 's|#include "WindVaneMenu.h"|#include "MainMenu.h"|g' "$file"
    
    echo "Updated includes in $file"
}

# Update all source files
find lib src test -name "*.h" -o -name "*.cpp" | while read file; do
    update_includes_standard_cpp "$file"
done
```

### Script 3: Update Header Guards
```bash
#!/bin/bash
# update_header_guards_standard_cpp.sh

update_header_guard_standard_cpp() {
    local file=$1
    local class_name=$(basename "$file" .h)
    local guard_name="${class_name^^}_H"
    
    # Replace header guard
    sed -i "1s/.*/#ifndef ${guard_name}/" "$file"
    sed -i "2s/.*/#define ${guard_name}/" "$file"
    sed -i "$ s/.*/\n#endif \/\/ ${guard_name}/" "$file"
    
    echo "Updated header guard in $file to $guard_name"
}

# Update all header files
find lib src -name "*.h" | while read file; do
    update_header_guard_standard_cpp "$file"
done
```

## Build Configuration

### Updated platformio.ini
```ini
; PlatformIO Project Configuration File - Standard C++ Naming
[env]
test_framework = googletest
build_unflags = -std=gnu++11

[env:native]
platform = native
build_type = test
lib_deps =
    ArduinoFake
    google/googletest@^1.12.1
build_flags =
    -I${PROJECT_DIR}/lib/Interfaces
    -I${PROJECT_DIR}/lib/Core
    -I${PROJECT_DIR}/lib/Hardware
    -I${PROJECT_DIR}/lib/Storage
    -I${PROJECT_DIR}/lib/UI
    -I${PROJECT_DIR}/lib/Services
    -std=gnu++20

[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
upload_speed = 921600
board_build.filesystem = littlefs
build_flags =
    -I${PROJECT_DIR}/lib/Interfaces
    -I${PROJECT_DIR}/lib/Core
    -I${PROJECT_DIR}/lib/Hardware
    -I${PROJECT_DIR}/lib/Storage
    -I${PROJECT_DIR}/lib/UI
    -I${PROJECT_DIR}/lib/Services
    -std=gnu++17
```

## Benefits of Standard C++ Naming

1. **Familiar Conventions**: Follows traditional C++ patterns most developers know
2. **Class-File Mapping**: Direct correspondence between class names and file names
3. **IDE Compatibility**: Works well with most C++ IDEs and tools
4. **Industry Standard**: Widely used in commercial C++ projects
5. **Clear Hierarchy**: Interface prefix makes abstractions obvious
6. **Maintainability**: Easy to locate files based on class names

## Migration Strategy

### Phase 1: File Renaming
1. Rename files to match class names exactly
2. Update directory structure to logical groupings
3. Change extensions from any non-standard to `.cpp/.h`

### Phase 2: Update References
1. Update all include statements
2. Fix header guards to match new names
3. Update build system paths

### Phase 3: Validation
1. Ensure all files compile successfully
2. Run test suite to verify functionality
3. Check that naming conventions are consistent

This approach provides a clean, standard C++ naming convention that makes the codebase familiar to most C++ developers while maintaining clear class-to-file relationships.