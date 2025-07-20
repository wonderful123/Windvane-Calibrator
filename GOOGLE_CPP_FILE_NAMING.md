# Google C++ Style Guide Compliant File Naming

## Overview
This document outlines the implementation of Google C++ Style Guide file naming conventions for the Wind Vane Calibrator embedded project.

## Google C++ Style Guide Requirements

### File Naming Rules
1. **All lowercase** with underscores (`_`) or dashes (`-`)
2. **Prefer underscores** over dashes
3. **Headers end in `.h`**
4. **Source files end in `.cc`** (NOT `.cpp`)
5. **Descriptive names** - avoid abbreviations
6. **Test files** use `_test.cc` suffix
7. **Pair naming**: `foo_bar.h` and `foo_bar.cc` for class `FooBar`

### Specific Requirements for Embedded Projects
- Use descriptive, full words instead of abbreviations
- Follow the pattern: functionality + component type
- Maintain consistency within the project
- Consider the 8.3 filename limitations if needed for embedded toolchains

## Current → Google C++ Compliant Mapping

### Core Interface Files
| Current File | Google C++ Compliant | Class Name | Reason |
|--------------|---------------------|------------|---------|
| `IADC.h` | `analog_digital_converter.h` | `AnalogDigitalConverter` | Descriptive, no abbreviation |
| `IIO.h` | `user_input_output.h` | `UserInputOutput` | Clear interface purpose |
| `ICalibrationStorage.h` | `calibration_storage.h` | `CalibrationStorage` | Remove redundant `I` prefix |
| `IDiagnostics.h` | `diagnostics.h` | `Diagnostics` | Simple, clear interface |
| `IPlatform.h` | `platform.h` | `Platform` | Hardware abstraction layer |

### Implementation Files  
| Current File | Google C++ Compliant | Class Name | Reason |
|--------------|---------------------|------------|---------|
| `ESP32ADC.cpp/.h` | `esp32_analog_converter.cc/.h` | `Esp32AnalogConverter` | Platform + component |
| `NullADC.h` | `mock_analog_converter.h` | `MockAnalogConverter` | Purpose is clear |
| `WindVane.cpp/.h` | `wind_vane.cc/.h` | `WindVane` | Main domain object |
| `CalibrationManager.cpp/.h` | `calibration_manager.cc/.h` | `CalibrationManager` | Manager pattern |
| `ClusterManager.cpp/.h` | `cluster_manager.cc/.h` | `ClusterManager` | Data clustering |

### Storage Components
| Current File | Google C++ Compliant | Class Name | Reason |
|--------------|---------------------|------------|---------|
| `EEPROMCalibrationStorage.cpp/.h` | `eeprom_calibration_storage.cc/.h` | `EepromCalibrationStorage` | Specific storage type |
| `FileCalibrationStorage.cpp/.h` | `file_calibration_storage.cc/.h` | `FileCalibrationStorage` | File-based storage |
| `SettingsManager.cpp/.h` | `settings_manager.cc/.h` | `SettingsManager` | Configuration management |

### UI Components
| Current File | Google C++ Compliant | Class Name | Reason |
|--------------|---------------------|------------|---------|
| `WindVaneMenu.cpp/.h` | `main_menu.cc/.h` | `MainMenu` | Primary interface |
| `DiagnosticsMenu.cpp/.h` | `diagnostics_menu.cc/.h` | `DiagnosticsMenu` | Specific menu type |
| `SettingsMenu.cpp/.h` | `settings_menu.cc/.h` | `SettingsMenu` | Configuration interface |
| `MenuDisplayController.cpp/.h` | `menu_display_controller.cc/.h` | `MenuDisplayController` | UI controller |

### Calibration Components
| Current File | Google C++ Compliant | Class Name | Reason |
|--------------|---------------------|------------|---------|
| `SpinningMethod.cpp/.h` | `spinning_calibration_strategy.cc/.h` | `SpinningCalibrationStrategy` | Strategy pattern clarity |
| `StrategyFactory.cpp/.h` | `calibration_strategy_factory.cc/.h` | `CalibrationStrategyFactory` | Factory pattern |

### Test Files
| Current File | Google C++ Compliant | Reason |
|--------------|---------------------|---------|
| `test_calibration/ClusterManager.cpp` | `cluster_manager_test.cc` | Google test naming |
| `test_io/IOHandlers.cpp` | `input_output_handlers_test.cc` | Descriptive test name |
| `test_storage/FileCalibrationStorage.cpp` | `file_calibration_storage_test.cc` | Clear test purpose |
| `test_hardwareinterface/ESP32ADC.cpp` | `esp32_analog_converter_test.cc` | Hardware test clarity |

## Directory Structure (Google C++ Compliant)

```
lib/
├── wind_vane/
│   ├── core/
│   │   ├── wind_vane.h
│   │   ├── wind_vane.cc
│   │   ├── calibration/
│   │   │   ├── calibration_manager.h
│   │   │   ├── calibration_manager.cc
│   │   │   ├── cluster_manager.h
│   │   │   ├── cluster_manager.cc
│   │   │   └── strategies/
│   │   │       ├── spinning_calibration_strategy.h
│   │   │       ├── spinning_calibration_strategy.cc
│   │   │       ├── calibration_strategy_factory.h
│   │   │       └── calibration_strategy_factory.cc
│   │   └── measurement/
│   │       ├── direction_calculator.h
│   │       └── direction_calculator.cc
│   ├── interfaces/
│   │   ├── analog_digital_converter.h
│   │   ├── calibration_storage.h
│   │   ├── user_input_output.h
│   │   ├── diagnostics.h
│   │   └── platform.h
│   ├── drivers/
│   │   ├── esp32/
│   │   │   ├── esp32_analog_converter.h
│   │   │   ├── esp32_analog_converter.cc
│   │   │   ├── esp32_serial_interface.h
│   │   │   └── esp32_serial_interface.cc
│   │   └── host/
│   │       ├── mock_analog_converter.h
│   │       ├── mock_analog_converter.cc
│   │       ├── console_interface.h
│   │       └── console_interface.cc
│   ├── storage/
│   │   ├── eeprom_calibration_storage.h
│   │   ├── eeprom_calibration_storage.cc
│   │   ├── file_calibration_storage.h
│   │   ├── file_calibration_storage.cc
│   │   ├── settings_manager.h
│   │   └── settings_manager.cc
│   ├── ui/
│   │   ├── main_menu.h
│   │   ├── main_menu.cc
│   │   ├── diagnostics_menu.h
│   │   ├── diagnostics_menu.cc
│   │   ├── settings_menu.h
│   │   ├── settings_menu.cc
│   │   ├── menu_display_controller.h
│   │   └── menu_display_controller.cc
│   └── services/
│       ├── dependency_container.h
│       ├── dependency_container.cc
│       ├── platform_factory.h
│       └── platform_factory.cc
├── platform/
│   ├── time_utilities.h
│   ├── time_utilities.cc
│   ├── platform_interface.h
│   └── platform_interface.cc
└── external/
    └── (third-party libraries)

src/
├── application.h
├── application.cc
├── main.cc
└── configuration.h

test/
├── unit/
│   ├── core/
│   │   ├── wind_vane_test.cc
│   │   ├── calibration/
│   │   │   ├── cluster_manager_test.cc
│   │   │   ├── calibration_manager_test.cc
│   │   │   └── spinning_calibration_strategy_test.cc
│   │   └── measurement/
│   │       └── direction_calculator_test.cc
│   ├── storage/
│   │   ├── file_calibration_storage_test.cc
│   │   ├── eeprom_calibration_storage_test.cc
│   │   └── settings_manager_test.cc
│   ├── drivers/
│   │   ├── esp32_analog_converter_test.cc
│   │   └── mock_analog_converter_test.cc
│   └── ui/
│       ├── main_menu_test.cc
│       ├── diagnostics_menu_test.cc
│       └── menu_display_controller_test.cc
├── integration/
│   ├── calibration_workflow_test.cc
│   ├── menu_navigation_test.cc
│   └── hardware_integration_test.cc
└── fixtures/
    ├── test_data.h
    ├── test_data.cc
    ├── mock_implementations.h
    └── mock_implementations.cc
```

## Implementation Guide

### Step 1: Create New Google C++ Compliant Files

Create the new file structure alongside existing files:

```bash
# Core interfaces
mkdir -p lib/wind_vane/interfaces
touch lib/wind_vane/interfaces/analog_digital_converter.h
touch lib/wind_vane/interfaces/calibration_storage.h
touch lib/wind_vane/interfaces/user_input_output.h
touch lib/wind_vane/interfaces/diagnostics.h
touch lib/wind_vane/interfaces/platform.h

# Core implementation
mkdir -p lib/wind_vane/core/calibration/strategies
touch lib/wind_vane/core/wind_vane.h
touch lib/wind_vane/core/wind_vane.cc
touch lib/wind_vane/core/calibration/calibration_manager.h
touch lib/wind_vane/core/calibration/calibration_manager.cc

# Drivers
mkdir -p lib/wind_vane/drivers/{esp32,host}
touch lib/wind_vane/drivers/esp32/esp32_analog_converter.h
touch lib/wind_vane/drivers/esp32/esp32_analog_converter.cc
touch lib/wind_vane/drivers/host/mock_analog_converter.h
touch lib/wind_vane/drivers/host/mock_analog_converter.cc

# Tests
mkdir -p test/unit/{core,storage,drivers,ui}
touch test/unit/core/wind_vane_test.cc
touch test/unit/storage/file_calibration_storage_test.cc
touch test/unit/drivers/esp32_analog_converter_test.cc
```

### Step 2: Update Build System

Update `platformio.ini`:
```ini
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
    -I${PROJECT_DIR}/lib/wind_vane/interfaces
    -I${PROJECT_DIR}/lib/wind_vane/core
    -I${PROJECT_DIR}/lib/wind_vane/drivers
    -I${PROJECT_DIR}/lib/wind_vane/storage
    -I${PROJECT_DIR}/lib/wind_vane/ui
    -I${PROJECT_DIR}/lib/wind_vane/services
    -I${PROJECT_DIR}/lib/platform
    -std=gnu++20

[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
upload_speed = 921600
board_build.filesystem = littlefs
build_flags =
    -I${PROJECT_DIR}/lib/wind_vane/interfaces
    -I${PROJECT_DIR}/lib/wind_vane/core
    -I${PROJECT_DIR}/lib/wind_vane/drivers
    -I${PROJECT_DIR}/lib/wind_vane/storage
    -I${PROJECT_DIR}/lib/wind_vane/ui
    -I${PROJECT_DIR}/lib/wind_vane/services
    -I${PROJECT_DIR}/lib/platform
    -std=gnu++17
```

### Step 3: Header Guards (Google C++ Format)

Use proper header guard format: `PROJECT_PATH_FILE_H_`

```cpp
// lib/wind_vane/core/wind_vane.h
#ifndef WIND_VANE_CORE_WIND_VANE_H_
#define WIND_VANE_CORE_WIND_VANE_H_

// ... content ...

#endif  // WIND_VANE_CORE_WIND_VANE_H_
```

### Step 4: Include Order (Google C++ Standard)

```cpp
#include "wind_vane/core/wind_vane.h"  // Related header first

#include <sys/types.h>                 // C system headers
#include <unistd.h>

#include <algorithm>                   // C++ standard library
#include <string>
#include <vector>

#include "wind_vane/interfaces/analog_digital_converter.h"  // Project headers
#include "wind_vane/interfaces/calibration_storage.h"
```

## Migration Scripts

### Script 1: Rename Files to Google C++ Compliance
```bash
#!/bin/bash
# rename_to_google_cpp.sh

# Function to rename a file pair
rename_file_pair() {
    local old_name=$1
    local new_name=$2
    local old_dir=$3
    local new_dir=$4
    
    if [ -f "${old_dir}/${old_name}.h" ]; then
        mkdir -p "$new_dir"
        mv "${old_dir}/${old_name}.h" "${new_dir}/${new_name}.h"
        echo "Renamed ${old_name}.h → ${new_name}.h"
    fi
    
    if [ -f "${old_dir}/${old_name}.cpp" ]; then
        mv "${old_dir}/${old_name}.cpp" "${new_dir}/${new_name}.cc"
        echo "Renamed ${old_name}.cpp → ${new_name}.cc"
    fi
}

# Core renames
rename_file_pair "WindVane" "wind_vane" "lib/WindVane" "lib/wind_vane/core"
rename_file_pair "CalibrationManager" "calibration_manager" "lib/WindVane/Calibration" "lib/wind_vane/core/calibration"
rename_file_pair "ClusterManager" "cluster_manager" "lib/WindVane/Calibration" "lib/wind_vane/core/calibration"

# Driver renames  
rename_file_pair "ADC" "esp32_analog_converter" "lib/WindVane/Drivers/ESP32" "lib/wind_vane/drivers/esp32"

# Interface renames (header only)
if [ -f "lib/WindVane/IADC.h" ]; then
    mkdir -p "lib/wind_vane/interfaces"
    mv "lib/WindVane/IADC.h" "lib/wind_vane/interfaces/analog_digital_converter.h"
fi

# Test renames
if [ -f "test/test_calibration/ClusterManager.cpp" ]; then
    mkdir -p "test/unit/core/calibration"
    mv "test/test_calibration/ClusterManager.cpp" "test/unit/core/calibration/cluster_manager_test.cc"
fi

echo "File renaming complete!"
```

### Script 2: Update Include Statements
```bash
#!/bin/bash
# update_includes_google_cpp.sh

update_includes() {
    local file=$1
    
    # Update old includes to new Google C++ style
    sed -i 's|#include <IADC.h>|#include "wind_vane/interfaces/analog_digital_converter.h"|g' "$file"
    sed -i 's|#include <WindVane.h>|#include "wind_vane/core/wind_vane.h"|g' "$file"
    sed -i 's|#include <CalibrationManager.h>|#include "wind_vane/core/calibration/calibration_manager.h"|g' "$file"
    sed -i 's|#include ".*\.cpp"|// Error: Do not include .cpp files|g' "$file"
    
    echo "Updated includes in $file"
}

# Update all source files
find lib src test -name "*.h" -o -name "*.cc" | while read file; do
    update_includes "$file"
done
```

### Script 3: Update Header Guards
```bash
#!/bin/bash
# update_header_guards.sh

update_header_guard() {
    local file=$1
    local guard_name=$(echo "$file" | sed 's|lib/||' | sed 's|src/||' | tr '/' '_' | tr '[:lower:]' '[:upper:]' | sed 's|\.H$|_H_|')
    
    # Remove old guards and add new ones
    sed -i '/^#ifndef.*_H_*$/d' "$file"
    sed -i '/^#define.*_H_*$/d' "$file"
    sed -i '/^#endif.*\/\/.*_H_*$/d' "$file"
    
    # Add proper Google C++ style guards
    sed -i "1i#ifndef ${guard_name}" "$file"
    sed -i "2i#define ${guard_name}" "$file"
    echo "" >> "$file"
    echo "#endif  // ${guard_name}" >> "$file"
    
    echo "Updated header guard in $file to $guard_name"
}

# Update all header files
find lib src -name "*.h" | while read file; do
    update_header_guard "$file"
done
```

## Benefits of Google C++ Compliant Naming

1. **Industry Standard**: Follows widely adopted conventions
2. **Tool Compatibility**: Works better with Google tools (Bazel, etc.)
3. **Consistency**: Uniform naming across large codebases
4. **Clarity**: Descriptive names improve readability
5. **Maintainability**: Easier for teams familiar with Google standards
6. **Integration**: Better compatibility with Google libraries/frameworks

## Migration Timeline

- **Week 1**: Script development and testing
- **Week 2**: File structure creation and validation
- **Week 3**: Build system updates and testing
- **Week 4**: Include path migration and validation
- **Week 5**: Final cleanup and documentation

This approach ensures compliance with Google C++ Style Guide while maintaining project functionality during the transition.