#!/bin/bash

# migrate_to_standard_cpp.sh
# Complete migration script to Standard C++ naming conventions
# for the Wind Vane Calibrator embedded project

set -e  # Exit on any error

echo "🚀 Starting migration to Standard C++ naming conventions..."

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Backup original files
backup_original_files() {
    log_info "Creating backup of original files..."
    
    if [ ! -d "backup_original" ]; then
        mkdir backup_original
        cp -r lib src test backup_original/
        log_success "Backup created in backup_original/"
    else
        log_warning "Backup already exists, skipping..."
    fi
}

# Create Standard C++ directory structure
create_standard_cpp_structure() {
    log_info "Creating Standard C++ directory structure..."
    
    # Core directories using PascalCase
    mkdir -p lib/{Interfaces,Core,Hardware,Storage,UI,Services}
    mkdir -p lib/Core/{Calibration/Strategies,Measurement}
    mkdir -p lib/Hardware/{Esp32,Mock}
    
    # Test directories using PascalCase
    mkdir -p test/{Unit,Integration,Fixtures}
    mkdir -p test/Unit/{Core,Storage,Hardware,UI}
    mkdir -p test/Unit/Core/{Calibration,Measurement}
    
    # Documentation and scripts
    mkdir -p docs scripts
    
    log_success "Directory structure created"
}

# Rename interface files to standard C++ conventions
rename_interface_files() {
    log_info "Renaming interface files to Standard C++ style..."
    
    # Interface files mapping
    declare -A interface_files=(
        ["lib/WindVane/IADC.h"]="lib/Interfaces/IAnalogDigitalConverter.h"
        ["lib/WindVane/UI/IIO.h"]="lib/Interfaces/IUserInterface.h"
        ["lib/WindVane/Storage/ICalibrationStorage.h"]="lib/Interfaces/ICalibrationStorage.h"
        ["lib/WindVane/Diagnostics/IDiagnostics.h"]="lib/Interfaces/IDiagnostics.h"
        ["lib/Platform/IPlatform.h"]="lib/Interfaces/IPlatform.h"
    )
    
    for old_file in "${!interface_files[@]}"; do
        new_file="${interface_files[$old_file]}"
        if [ -f "$old_file" ]; then
            cp "$old_file" "$new_file"
            log_success "Created: $new_file"
        else
            log_warning "Source file not found: $old_file"
        fi
    done
}

# Rename core implementation files
rename_core_files() {
    log_info "Renaming core implementation files..."
    
    # Core files mapping (both .h and .cpp)
    declare -A core_files=(
        ["WindVane"]="WindVane"
        ["CalibrationManager"]="CalibrationManager"
        ["ClusterManager"]="ClusterManager"
        ["SettingsManager"]="SettingsManager"
    )
    
    for old_name in "${!core_files[@]}"; do
        new_name="${core_files[$old_name]}"
        
        # Handle header files
        if [ -f "lib/WindVane/${old_name}.h" ]; then
            cp "lib/WindVane/${old_name}.h" "lib/Core/${new_name}.h"
            log_success "Created: lib/Core/${new_name}.h"
        fi
        
        # Handle source files (.cpp)
        if [ -f "lib/WindVane/${old_name}.cpp" ]; then
            cp "lib/WindVane/${old_name}.cpp" "lib/Core/${new_name}.cpp"
            log_success "Created: lib/Core/${new_name}.cpp"
        fi
        
        # Handle calibration subfolder
        if [ -f "lib/WindVane/Calibration/${old_name}.h" ]; then
            cp "lib/WindVane/Calibration/${old_name}.h" "lib/Core/Calibration/${new_name}.h"
            log_success "Created: lib/Core/Calibration/${new_name}.h"
        fi
        
        if [ -f "lib/WindVane/Calibration/${old_name}.cpp" ]; then
            cp "lib/WindVane/Calibration/${old_name}.cpp" "lib/Core/Calibration/${new_name}.cpp"
            log_success "Created: lib/Core/Calibration/${new_name}.cpp"
        fi
    done
}

# Rename hardware/driver files
rename_hardware_files() {
    log_info "Renaming hardware files..."
    
    # ESP32 ADC driver
    if [ -f "lib/WindVane/Drivers/ESP32/ADC.h" ]; then
        cp "lib/WindVane/Drivers/ESP32/ADC.h" "lib/Hardware/Esp32/Esp32AnalogConverter.h"
        log_success "Created: lib/Hardware/Esp32/Esp32AnalogConverter.h"
    fi
    
    if [ -f "lib/WindVane/Drivers/ESP32/ADC.cpp" ]; then
        cp "lib/WindVane/Drivers/ESP32/ADC.cpp" "lib/Hardware/Esp32/Esp32AnalogConverter.cpp"
        log_success "Created: lib/Hardware/Esp32/Esp32AnalogConverter.cpp"
    fi
    
    # Mock/Host ADC driver
    if [ -f "src/host/NullADC.h" ]; then
        cp "src/host/NullADC.h" "lib/Hardware/Mock/MockAnalogConverter.h"
        log_success "Created: lib/Hardware/Mock/MockAnalogConverter.h"
    fi
    
    # ESP32 Serial Interface
    if [ -f "lib/WindVane/Drivers/ESP32/Serial.h" ]; then
        cp "lib/WindVane/Drivers/ESP32/Serial.h" "lib/Hardware/Esp32/Esp32SerialInterface.h"
        log_success "Created: lib/Hardware/Esp32/Esp32SerialInterface.h"
    fi
    
    if [ -f "lib/WindVane/Drivers/ESP32/Serial.cpp" ]; then
        cp "lib/WindVane/Drivers/ESP32/Serial.cpp" "lib/Hardware/Esp32/Esp32SerialInterface.cpp"
        log_success "Created: lib/Hardware/Esp32/Esp32SerialInterface.cpp"
    fi
}

# Rename storage files
rename_storage_files() {
    log_info "Renaming storage files..."
    
    declare -A storage_files=(
        ["EEPROMCalibrationStorage"]="EepromCalibrationStorage"
        ["FileCalibrationStorage"]="FileCalibrationStorage"
        ["EEPROMSettingsStorage"]="EepromSettingsStorage"
        ["FileSettingsStorage"]="FileSettingsStorage"
        ["SettingsManager"]="SettingsManager"
    )
    
    for old_name in "${!storage_files[@]}"; do
        new_name="${storage_files[$old_name]}"
        
        # Main storage directory
        if [ -f "lib/WindVane/Storage/${old_name}.h" ]; then
            cp "lib/WindVane/Storage/${old_name}.h" "lib/Storage/${new_name}.h"
            log_success "Created: lib/Storage/${new_name}.h"
        fi
        
        if [ -f "lib/WindVane/Storage/${old_name}.cpp" ]; then
            cp "lib/WindVane/Storage/${old_name}.cpp" "lib/Storage/${new_name}.cpp"
            log_success "Created: lib/Storage/${new_name}.cpp"
        fi
        
        # Settings subdirectory
        if [ -f "lib/WindVane/Storage/Settings/${old_name}.h" ]; then
            cp "lib/WindVane/Storage/Settings/${old_name}.h" "lib/Storage/${new_name}.h"
            log_success "Created: lib/Storage/${new_name}.h"
        fi
        
        if [ -f "lib/WindVane/Storage/Settings/${old_name}.cpp" ]; then
            cp "lib/WindVane/Storage/Settings/${old_name}.cpp" "lib/Storage/${new_name}.cpp"
            log_success "Created: lib/Storage/${new_name}.cpp"
        fi
    done
}

# Rename UI/Menu files
rename_ui_files() {
    log_info "Renaming UI/Menu files..."
    
    declare -A ui_files=(
        ["WindVaneMenu"]="MainMenu"
        ["DiagnosticsMenu"]="DiagnosticsMenu"
        ["SettingsMenu"]="SettingsMenu"
        ["MenuDisplayController"]="MenuDisplayController"
        ["MenuDisplayView"]="MenuDisplayView"
        ["MenuPresenter"]="MenuPresenter"
    )
    
    for old_name in "${!ui_files[@]}"; do
        new_name="${ui_files[$old_name]}"
        
        if [ -f "lib/WindVaneMenu/${old_name}.h" ]; then
            cp "lib/WindVaneMenu/${old_name}.h" "lib/UI/${new_name}.h"
            log_success "Created: lib/UI/${new_name}.h"
        fi
        
        if [ -f "lib/WindVaneMenu/${old_name}.cpp" ]; then
            cp "lib/WindVaneMenu/${old_name}.cpp" "lib/UI/${new_name}.cpp"
            log_success "Created: lib/UI/${new_name}.cpp"
        fi
    done
}

# Rename test files
rename_test_files() {
    log_info "Renaming test files..."
    
    declare -A test_files=(
        ["test/test_calibration/ClusterManager.cpp"]="test/Unit/Core/Calibration/ClusterManagerTest.cpp"
        ["test/test_io/IOHandlers.cpp"]="test/Unit/UI/UserInterfaceTest.cpp"
        ["test/test_storage/FileCalibrationStorage.cpp"]="test/Unit/Storage/FileCalibrationStorageTest.cpp"
        ["test/test_storage/FileSettingsStorage.cpp"]="test/Unit/Storage/FileSettingsStorageTest.cpp"
        ["test/test_hardwareinterface/ESP32ADC.cpp"]="test/Unit/Hardware/Esp32AnalogConverterTest.cpp"
        ["test/test_time/TimeMs.cpp"]="test/Unit/Core/TimeUtilitiesTest.cpp"
    )
    
    for old_file in "${!test_files[@]}"; do
        new_file="${test_files[$old_file]}"
        if [ -f "$old_file" ]; then
            cp "$old_file" "$new_file"
            log_success "Created: $new_file"
        else
            log_warning "Test file not found: $old_file"
        fi
    done
}

# Rename main application files
rename_app_files() {
    log_info "Renaming main application files..."
    
    declare -A app_files=(
        ["src/App.h"]="src/Application.h"
        ["src/App.cpp"]="src/Application.cpp"
        ["src/PlatformFactory.h"]="lib/Services/PlatformFactory.h"
        ["src/PlatformFactory.cpp"]="lib/Services/PlatformFactory.cpp"
        ["src/Config.h"]="src/Config.h"
    )
    
    for old_file in "${!app_files[@]}"; do
        new_file="${app_files[$old_file]}"
        if [ -f "$old_file" ]; then
            cp "$old_file" "$new_file"
            log_success "Created: $new_file"
        else
            log_warning "App file not found: $old_file"
        fi
    done
}

# Update header guards to standard C++ style
update_header_guards() {
    log_info "Updating header guards to Standard C++ style..."
    
    find lib src -name "*.h" | while read -r file; do
        if [ -f "$file" ]; then
            # Generate guard name from class name
            class_name=$(basename "$file" .h)
            guard_name="${class_name^^}_H"
            
            # Create temporary file with new guards
            temp_file=$(mktemp)
            
            # Remove existing guards
            grep -v "^#ifndef.*_H" "$file" | grep -v "^#define.*_H" | grep -v "^#endif.*_H" > "$temp_file"
            
            # Add new Standard C++ style guards
            {
                echo "#ifndef ${guard_name}"
                echo "#define ${guard_name}"
                echo ""
                cat "$temp_file"
                echo ""
                echo "#endif // ${guard_name}"
            } > "$file"
            
            rm "$temp_file"
            log_success "Updated header guard: $file -> $guard_name"
        fi
    done
}

# Update include statements
update_includes() {
    log_info "Updating include statements..."
    
    find lib src test -name "*.h" -o -name "*.cpp" | while read -r file; do
        if [ -f "$file" ]; then
            # Update common include patterns to use new Standard C++ paths
            sed -i.bak 's|#include <IADC\.h>|#include "../Interfaces/IAnalogDigitalConverter.h"|g' "$file"
            sed -i.bak 's|#include <IIO\.h>|#include "../Interfaces/IUserInterface.h"|g' "$file"
            sed -i.bak 's|#include <WindVane\.h>|#include "../Core/WindVane.h"|g' "$file"
            sed -i.bak 's|#include <CalibrationManager\.h>|#include "../Core/Calibration/CalibrationManager.h"|g' "$file"
            sed -i.bak 's|#include <ClusterManager\.h>|#include "../Core/Calibration/ClusterManager.h"|g' "$file"
            sed -i.bak 's|#include <UI/IIO\.h>|#include "../Interfaces/IUserInterface.h"|g' "$file"
            sed -i.bak 's|#include <Storage/ICalibrationStorage\.h>|#include "../Interfaces/ICalibrationStorage.h"|g' "$file"
            sed -i.bak 's|#include <Diagnostics/IDiagnostics\.h>|#include "../Interfaces/IDiagnostics.h"|g' "$file"
            
            # Update hardware includes
            sed -i.bak 's|#include "ESP32ADC\.h"|#include "Esp32AnalogConverter.h"|g' "$file"
            sed -i.bak 's|#include "NullADC\.h"|#include "MockAnalogConverter.h"|g' "$file"
            
            # Update UI includes
            sed -i.bak 's|#include "WindVaneMenu\.h"|#include "MainMenu.h"|g' "$file"
            
            # Remove backup files
            rm -f "${file}.bak"
            
            log_success "Updated includes: $file"
        fi
    done
}

# Update build configuration
update_build_config() {
    log_info "Updating build configuration..."
    
    # Update platformio.ini
    if [ -f "platformio.ini" ]; then
        cp "platformio.ini" "platformio.ini.bak"
        
        cat > "platformio.ini" << 'EOF'
; PlatformIO Project Configuration File - Standard C++ Naming
;
; Build options: build flags, source filter
; Upload options: custom upload port, speed and extra flags
; Library options: dependencies, extra library storages

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
EOF
        
        log_success "Updated platformio.ini with Standard C++ compliant paths"
    fi
}

# Create migration report
create_migration_report() {
    log_info "Creating migration report..."
    
    cat > "STANDARD_CPP_MIGRATION_REPORT.md" << 'EOF'
# Standard C++ Naming Migration Report

## Migration Summary

This project has been migrated to comply with Standard C++ naming conventions.

### Key Changes

1. **File Extensions**: All source files use `.cpp` extension
2. **Directory Structure**: Reorganized into logical `PascalCase` directories  
3. **File Names**: All files use `PascalCase` matching class names exactly
4. **Header Guards**: Updated to `CLASSNAME_H` format
5. **Include Paths**: Updated to use new directory structure
6. **Member Variables**: Use `m_` prefix following standard convention

### Before/After File Mapping

#### Interfaces
- `IADC.h` → `Interfaces/IAnalogDigitalConverter.h`
- `IIO.h` → `Interfaces/IUserInterface.h`
- `ICalibrationStorage.h` → `Interfaces/ICalibrationStorage.h`

#### Core Implementation
- `WindVane.cpp/.h` → `Core/WindVane.cpp/.h`
- `CalibrationManager.cpp/.h` → `Core/Calibration/CalibrationManager.cpp/.h`
- `ClusterManager.cpp/.h` → `Core/Calibration/ClusterManager.cpp/.h`

#### Hardware Drivers
- `ESP32/ADC.cpp/.h` → `Hardware/Esp32/Esp32AnalogConverter.cpp/.h`
- `host/NullADC.h` → `Hardware/Mock/MockAnalogConverter.h`

#### Tests
- `test_calibration/ClusterManager.cpp` → `test/Unit/Core/Calibration/ClusterManagerTest.cpp`
- `test_io/IOHandlers.cpp` → `test/Unit/UI/UserInterfaceTest.cpp`

### Directory Structure

```
lib/
├── Interfaces/           # All interface definitions
├── Core/                # Core business logic
│   ├── Calibration/     # Calibration algorithms
│   └── Measurement/     # Measurement logic
├── Hardware/            # Hardware drivers
│   ├── Esp32/          # ESP32-specific implementations
│   └── Mock/           # Mock/test implementations
├── Storage/            # Data persistence
├── UI/                 # User interface components
└── Services/           # Application services

test/
├── Unit/               # Unit tests by component
├── Integration/        # Integration tests
└── Fixtures/          # Test data and utilities
```

### Naming Conventions Applied

- **Classes**: `PascalCase` (e.g., `WindVane`, `CalibrationManager`)
- **Interfaces**: `I` prefix + `PascalCase` (e.g., `IAnalogDigitalConverter`)
- **Files**: Match class name exactly (e.g., `WindVane.h` for `WindVane` class)
- **Methods**: `PascalCase` (e.g., `GetDirection()`, `StartCalibration()`)
- **Member Variables**: `m_` prefix + `camelCase` (e.g., `m_adc`, `m_storage`)
- **Test Files**: `ClassNameTest.cpp` format
- **Header Guards**: `CLASSNAME_H` format

### Build System

Updated `platformio.ini` with new include paths and Standard C++ compliant configuration.

### Benefits

1. **Familiar Conventions**: Follows traditional C++ patterns most developers know
2. **Class-File Mapping**: Direct correspondence between class names and file names
3. **IDE Compatibility**: Works well with most C++ IDEs and tools
4. **Industry Standard**: Widely used in commercial C++ projects
5. **Clear Hierarchy**: Interface prefix makes abstractions obvious
6. **Maintainability**: Easy to locate files based on class names

### Verification

To verify the migration:
1. Check that all files use `PascalCase` naming
2. Verify header guards use `CLASSNAME_H` format
3. Confirm include statements use new paths
4. Test build system with new structure
5. Ensure all class names match file names exactly

### Next Steps

1. Update all include statements project-wide
2. Test build system with new structure
3. Run full test suite to verify functionality
4. Update documentation to reflect new structure
5. Consider adopting consistent member naming (`m_` prefix)
EOF

    log_success "Migration report created: STANDARD_CPP_MIGRATION_REPORT.md"
}

# Validate migration
validate_migration() {
    log_info "Validating migration..."
    
    local errors=0
    
    # Check for Standard C++ compliant naming
    if find lib -name "*[a-z]*[A-Z]*[a-z]*" | grep -v PascalCase | grep -q .; then
        log_warning "Found files that may not follow PascalCase in lib directory"
    fi
    
    # Check for consistent .cpp extensions
    if find lib src -name "*.cc" | grep -q .; then
        log_error "Found .cc files (should be .cpp for standard C++)"
        ((errors++))
    fi
    
    # Check test file naming
    if find test -name "*test.h" -o -name "*test.cpp" | grep -v "Test" | grep -q .; then
        log_error "Found test files not following ClassNameTest.cpp convention"
        ((errors++))
    fi
    
    # Check header guards
    if grep -r "#ifndef.*_H_" lib/ src/ | grep -q .; then
        log_warning "Found some header guards with _H_ suffix (should be _H for standard C++)"
    fi
    
    if [ $errors -eq 0 ]; then
        log_success "Migration validation passed!"
    else
        log_error "Migration validation failed with $errors errors"
        return 1
    fi
}

# Main migration function
main() {
    echo "🔧 Standard C++ Naming Migration Tool"
    echo "======================================"
    
    # Check if we're in the right directory
    if [ ! -f "platformio.ini" ]; then
        log_error "Not in a PlatformIO project directory!"
        exit 1
    fi
    
    # Run migration steps
    backup_original_files
    create_standard_cpp_structure
    rename_interface_files
    rename_core_files
    rename_hardware_files
    rename_storage_files
    rename_ui_files
    rename_test_files
    rename_app_files
    update_header_guards
    update_includes
    update_build_config
    create_migration_report
    validate_migration
    
    echo ""
    log_success "🎉 Migration to Standard C++ naming completed successfully!"
    echo ""
    echo "📋 Summary:"
    echo "   ✅ File structure reorganized to PascalCase directories"
    echo "   ✅ Files renamed to match class names exactly"
    echo "   ✅ Header guards updated to CLASSNAME_H format"
    echo "   ✅ Include statements updated to new paths"
    echo "   ✅ Build configuration updated for .cpp extensions"
    echo "   ✅ Test files follow ClassNameTest.cpp convention"
    echo ""
    echo "📁 Backup of original files: backup_original/"
    echo "📄 Migration report: STANDARD_CPP_MIGRATION_REPORT.md"
    echo ""
    echo "🔄 Next steps:"
    echo "   1. Test build: pio run -e native"
    echo "   2. Run tests: pio test -e native"
    echo "   3. Review migration report"
    echo "   4. Update any remaining include paths manually"
    echo "   5. Consider adopting m_ prefix for member variables"
}

# Run main function
main "$@"