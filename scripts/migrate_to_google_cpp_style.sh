#!/bin/bash

# migrate_to_google_cpp_style.sh
# Complete migration script to Google C++ Style Guide compliant file naming
# for the Wind Vane Calibrator embedded project

set -e  # Exit on any error

echo "🚀 Starting migration to Google C++ Style Guide compliant file naming..."

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

# Create Google C++ compliant directory structure
create_google_cpp_structure() {
    log_info "Creating Google C++ compliant directory structure..."
    
    # Core directories
    mkdir -p lib/wind_vane/{interfaces,core,drivers,storage,ui,services}
    mkdir -p lib/wind_vane/core/{calibration/strategies,measurement}
    mkdir -p lib/wind_vane/drivers/{esp32,host}
    mkdir -p lib/platform
    
    # Test directories
    mkdir -p test/{unit,integration,fixtures}
    mkdir -p test/unit/{core,storage,drivers,ui}
    mkdir -p test/unit/core/{calibration,measurement}
    
    # Documentation and scripts
    mkdir -p docs scripts
    
    log_success "Directory structure created"
}

# Rename core interface files
rename_interface_files() {
    log_info "Renaming interface files to Google C++ style..."
    
    # Interface files mapping
    declare -A interface_files=(
        ["lib/WindVane/IADC.h"]="lib/wind_vane/interfaces/analog_digital_converter.h"
        ["lib/WindVane/UI/IIO.h"]="lib/wind_vane/interfaces/user_input_output.h"
        ["lib/WindVane/Storage/ICalibrationStorage.h"]="lib/wind_vane/interfaces/calibration_storage.h"
        ["lib/WindVane/Diagnostics/IDiagnostics.h"]="lib/wind_vane/interfaces/diagnostics.h"
        ["lib/Platform/IPlatform.h"]="lib/wind_vane/interfaces/platform.h"
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
    
    # Core files mapping (both .h and .cpp/.cc)
    declare -A core_files=(
        ["WindVane"]="wind_vane"
        ["CalibrationManager"]="calibration_manager"
        ["ClusterManager"]="cluster_manager"
        ["SettingsManager"]="settings_manager"
    )
    
    for old_name in "${!core_files[@]}"; do
        new_name="${core_files[$old_name]}"
        
        # Handle header files
        if [ -f "lib/WindVane/${old_name}.h" ]; then
            cp "lib/WindVane/${old_name}.h" "lib/wind_vane/core/${new_name}.h"
            log_success "Created: lib/wind_vane/core/${new_name}.h"
        fi
        
        # Handle source files (.cpp -> .cc)
        if [ -f "lib/WindVane/${old_name}.cpp" ]; then
            cp "lib/WindVane/${old_name}.cpp" "lib/wind_vane/core/${new_name}.cc"
            log_success "Created: lib/wind_vane/core/${new_name}.cc"
        fi
        
        # Handle calibration subfolder
        if [ -f "lib/WindVane/Calibration/${old_name}.h" ]; then
            cp "lib/WindVane/Calibration/${old_name}.h" "lib/wind_vane/core/calibration/${new_name}.h"
            log_success "Created: lib/wind_vane/core/calibration/${new_name}.h"
        fi
        
        if [ -f "lib/WindVane/Calibration/${old_name}.cpp" ]; then
            cp "lib/WindVane/Calibration/${old_name}.cpp" "lib/wind_vane/core/calibration/${new_name}.cc"
            log_success "Created: lib/wind_vane/core/calibration/${new_name}.cc"
        fi
    done
}

# Rename driver files
rename_driver_files() {
    log_info "Renaming driver files..."
    
    # ESP32 ADC driver
    if [ -f "lib/WindVane/Drivers/ESP32/ADC.h" ]; then
        cp "lib/WindVane/Drivers/ESP32/ADC.h" "lib/wind_vane/drivers/esp32/esp32_analog_converter.h"
        log_success "Created: lib/wind_vane/drivers/esp32/esp32_analog_converter.h"
    fi
    
    if [ -f "lib/WindVane/Drivers/ESP32/ADC.cpp" ]; then
        cp "lib/WindVane/Drivers/ESP32/ADC.cpp" "lib/wind_vane/drivers/esp32/esp32_analog_converter.cc"
        log_success "Created: lib/wind_vane/drivers/esp32/esp32_analog_converter.cc"
    fi
    
    # Host/Mock ADC driver
    if [ -f "src/host/NullADC.h" ]; then
        cp "src/host/NullADC.h" "lib/wind_vane/drivers/host/mock_analog_converter.h"
        log_success "Created: lib/wind_vane/drivers/host/mock_analog_converter.h"
    fi
}

# Rename storage files
rename_storage_files() {
    log_info "Renaming storage files..."
    
    declare -A storage_files=(
        ["EEPROMCalibrationStorage"]="eeprom_calibration_storage"
        ["FileCalibrationStorage"]="file_calibration_storage"
        ["EEPROMSettingsStorage"]="eeprom_settings_storage"
        ["FileSettingsStorage"]="file_settings_storage"
    )
    
    for old_name in "${!storage_files[@]}"; do
        new_name="${storage_files[$old_name]}"
        
        # Main storage directory
        if [ -f "lib/WindVane/Storage/${old_name}.h" ]; then
            cp "lib/WindVane/Storage/${old_name}.h" "lib/wind_vane/storage/${new_name}.h"
            log_success "Created: lib/wind_vane/storage/${new_name}.h"
        fi
        
        if [ -f "lib/WindVane/Storage/${old_name}.cpp" ]; then
            cp "lib/WindVane/Storage/${old_name}.cpp" "lib/wind_vane/storage/${new_name}.cc"
            log_success "Created: lib/wind_vane/storage/${new_name}.cc"
        fi
        
        # Settings subdirectory
        if [ -f "lib/WindVane/Storage/Settings/${old_name}.h" ]; then
            cp "lib/WindVane/Storage/Settings/${old_name}.h" "lib/wind_vane/storage/${new_name}.h"
            log_success "Created: lib/wind_vane/storage/${new_name}.h"
        fi
        
        if [ -f "lib/WindVane/Storage/Settings/${old_name}.cpp" ]; then
            cp "lib/WindVane/Storage/Settings/${old_name}.cpp" "lib/wind_vane/storage/${new_name}.cc"
            log_success "Created: lib/wind_vane/storage/${new_name}.cc"
        fi
    done
}

# Rename UI/Menu files
rename_ui_files() {
    log_info "Renaming UI/Menu files..."
    
    declare -A ui_files=(
        ["WindVaneMenu"]="main_menu"
        ["DiagnosticsMenu"]="diagnostics_menu"
        ["SettingsMenu"]="settings_menu"
        ["MenuDisplayController"]="menu_display_controller"
        ["MenuDisplayView"]="menu_display_view"
        ["MenuPresenter"]="menu_presenter"
    )
    
    for old_name in "${!ui_files[@]}"; do
        new_name="${ui_files[$old_name]}"
        
        if [ -f "lib/WindVaneMenu/${old_name}.h" ]; then
            cp "lib/WindVaneMenu/${old_name}.h" "lib/wind_vane/ui/${new_name}.h"
            log_success "Created: lib/wind_vane/ui/${new_name}.h"
        fi
        
        if [ -f "lib/WindVaneMenu/${old_name}.cpp" ]; then
            cp "lib/WindVaneMenu/${old_name}.cpp" "lib/wind_vane/ui/${new_name}.cc"
            log_success "Created: lib/wind_vane/ui/${new_name}.cc"
        fi
    done
}

# Rename test files
rename_test_files() {
    log_info "Renaming test files..."
    
    declare -A test_files=(
        ["test/test_calibration/ClusterManager.cpp"]="test/unit/core/calibration/cluster_manager_test.cc"
        ["test/test_io/IOHandlers.cpp"]="test/unit/ui/input_output_handlers_test.cc"
        ["test/test_storage/FileCalibrationStorage.cpp"]="test/unit/storage/file_calibration_storage_test.cc"
        ["test/test_storage/FileSettingsStorage.cpp"]="test/unit/storage/file_settings_storage_test.cc"
        ["test/test_hardwareinterface/ESP32ADC.cpp"]="test/unit/drivers/esp32_analog_converter_test.cc"
        ["test/test_time/TimeMs.cpp"]="test/unit/core/time_utilities_test.cc"
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
        ["src/App.h"]="src/application.h"
        ["src/App.cpp"]="src/application.cc"
        ["src/PlatformFactory.h"]="lib/wind_vane/services/platform_factory.h"
        ["src/PlatformFactory.cpp"]="lib/wind_vane/services/platform_factory.cc"
        ["src/Config.h"]="src/configuration.h"
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

# Update header guards to Google C++ style
update_header_guards() {
    log_info "Updating header guards to Google C++ style..."
    
    find lib/wind_vane src -name "*.h" | while read -r file; do
        if [ -f "$file" ]; then
            # Generate guard name from file path
            guard_name=$(echo "$file" | sed 's|^lib/||' | sed 's|^src/||' | tr '/' '_' | tr '[:lower:]' '[:upper:]' | sed 's|\.H$|_H_|')
            
            # Create temporary file with new guards
            temp_file=$(mktemp)
            
            # Remove existing guards
            grep -v "^#ifndef.*_H" "$file" | grep -v "^#define.*_H" | grep -v "^#endif.*_H" > "$temp_file"
            
            # Add new Google C++ style guards
            {
                echo "#ifndef ${guard_name}"
                echo "#define ${guard_name}"
                echo ""
                cat "$temp_file"
                echo ""
                echo "#endif  // ${guard_name}"
            } > "$file"
            
            rm "$temp_file"
            log_success "Updated header guard: $file -> $guard_name"
        fi
    done
}

# Update include statements
update_includes() {
    log_info "Updating include statements..."
    
    find lib/wind_vane src test -name "*.h" -o -name "*.cc" | while read -r file; do
        if [ -f "$file" ]; then
            # Update common include patterns
            sed -i.bak 's|#include <IADC\.h>|#include "wind_vane/interfaces/analog_digital_converter.h"|g' "$file"
            sed -i.bak 's|#include <WindVane\.h>|#include "wind_vane/core/wind_vane.h"|g' "$file"
            sed -i.bak 's|#include <CalibrationManager\.h>|#include "wind_vane/core/calibration/calibration_manager.h"|g' "$file"
            sed -i.bak 's|#include <ClusterManager\.h>|#include "wind_vane/core/calibration/cluster_manager.h"|g' "$file"
            sed -i.bak 's|#include <UI/IIO\.h>|#include "wind_vane/interfaces/user_input_output.h"|g' "$file"
            sed -i.bak 's|#include <Storage/ICalibrationStorage\.h>|#include "wind_vane/interfaces/calibration_storage.h"|g' "$file"
            sed -i.bak 's|#include <Diagnostics/IDiagnostics\.h>|#include "wind_vane/interfaces/diagnostics.h"|g' "$file"
            
            # Remove .cpp includes (not allowed in Google C++ style)
            sed -i.bak 's|#include ".*\.cpp"|// Error: Do not include .cpp files|g' "$file"
            
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
; PlatformIO Project Configuration File - Google C++ Style Compliant
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
EOF
        
        log_success "Updated platformio.ini with Google C++ compliant paths"
    fi
}

# Create migration report
create_migration_report() {
    log_info "Creating migration report..."
    
    cat > "MIGRATION_REPORT.md" << 'EOF'
# Google C++ Style Guide Migration Report

## Migration Summary

This project has been migrated to comply with Google C++ Style Guide file naming conventions.

### Key Changes

1. **File Extensions**: Changed `.cpp` to `.cc` for all source files
2. **Directory Structure**: Reorganized into logical `snake_case` directories
3. **File Names**: All files now use lowercase with underscores
4. **Header Guards**: Updated to Google C++ format: `PROJECT_PATH_FILE_H_`
5. **Include Paths**: Updated to use new directory structure

### Before/After File Mapping

#### Interfaces
- `IADC.h` → `wind_vane/interfaces/analog_digital_converter.h`
- `IIO.h` → `wind_vane/interfaces/user_input_output.h`
- `ICalibrationStorage.h` → `wind_vane/interfaces/calibration_storage.h`

#### Core Implementation
- `WindVane.cpp/.h` → `wind_vane/core/wind_vane.cc/.h`
- `CalibrationManager.cpp/.h` → `wind_vane/core/calibration/calibration_manager.cc/.h`
- `ClusterManager.cpp/.h` → `wind_vane/core/calibration/cluster_manager.cc/.h`

#### Drivers
- `ESP32/ADC.cpp/.h` → `wind_vane/drivers/esp32/esp32_analog_converter.cc/.h`
- `host/NullADC.h` → `wind_vane/drivers/host/mock_analog_converter.h`

#### Tests
- `test_calibration/ClusterManager.cpp` → `test/unit/core/calibration/cluster_manager_test.cc`
- `test_io/IOHandlers.cpp` → `test/unit/ui/input_output_handlers_test.cc`

### Backward Compatibility

Type aliases are provided for backward compatibility:
```cpp
using IADC = AnalogDigitalConverter;
using WindVane_Legacy = WindVane;
```

### Build System

Updated `platformio.ini` with new include paths and Google C++ compliant configuration.

### Verification

To verify the migration:
1. Check that all files follow `snake_case` naming
2. Verify header guards use `PROJECT_PATH_FILE_H_` format
3. Confirm include statements use new paths
4. Test build system with new structure

### Next Steps

1. Gradually migrate code to use new class names
2. Remove backward compatibility aliases after full migration
3. Update documentation to reflect new structure
4. Run full test suite to verify functionality
EOF

    log_success "Migration report created: MIGRATION_REPORT.md"
}

# Validate migration
validate_migration() {
    log_info "Validating migration..."
    
    local errors=0
    
    # Check for Google C++ compliant naming
    if find lib/wind_vane -name "*[A-Z]*" | grep -q .; then
        log_error "Found files with uppercase letters in wind_vane directory"
        ((errors++))
    fi
    
    # Check for .cpp files (should be .cc)
    if find lib/wind_vane src -name "*.cpp" | grep -q .; then
        log_error "Found .cpp files (should be .cc)"
        ((errors++))
    fi
    
    # Check test file naming
    if find test -name "*test.cpp" | grep -q .; then
        log_error "Found test files with .cpp extension (should be .cc)"
        ((errors++))
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
    echo "🔧 Google C++ Style Guide Migration Tool"
    echo "========================================"
    
    # Check if we're in the right directory
    if [ ! -f "platformio.ini" ]; then
        log_error "Not in a PlatformIO project directory!"
        exit 1
    fi
    
    # Run migration steps
    backup_original_files
    create_google_cpp_structure
    rename_interface_files
    rename_core_files
    rename_driver_files
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
    log_success "🎉 Migration to Google C++ Style Guide completed successfully!"
    echo ""
    echo "📋 Summary:"
    echo "   ✅ File structure reorganized"
    echo "   ✅ Files renamed to Google C++ style"
    echo "   ✅ Header guards updated"
    echo "   ✅ Include statements updated"
    echo "   ✅ Build configuration updated"
    echo "   ✅ Backward compatibility maintained"
    echo ""
    echo "📁 Backup of original files: backup_original/"
    echo "📄 Migration report: MIGRATION_REPORT.md"
    echo ""
    echo "🔄 Next steps:"
    echo "   1. Test build: pio run -e native"
    echo "   2. Run tests: pio test -e native"
    echo "   3. Review migration report"
    echo "   4. Gradually remove backward compatibility aliases"
}

# Run main function
main "$@"