# File Naming Migration Guide

## Overview
This guide provides a step-by-step approach to migrate from the current file naming conventions to the improved structure while maintaining backward compatibility.

## Migration Strategy

### Phase 1: Create New Structure (✅ Completed)
- ✅ Created new directory structure under `lib/wind_vane/`
- ✅ Created improved interface files with clear names
- ✅ Added backward compatibility aliases
- ✅ Established new test organization

### Phase 2: Update Build Configuration
```cmake
# Add new include paths to CMakeLists.txt
target_include_directories(WindVaneLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/wind_vane/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/wind_vane/core
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/wind_vane/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/wind_vane/storage
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/wind_vane/ui
    # Legacy paths for backward compatibility
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/WindVane
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/WindVaneMenu
)
```

### Phase 3: Gradual Code Migration

#### 3.1 Update Include Statements
**Old includes:**
```cpp
#include <IADC.h>
#include <WindVane.h>
#include <Storage/ICalibrationStorage.h>
#include <UI/IIO.h>
```

**New includes:**
```cpp
#include <wind_vane/interfaces/analog_to_digital_converter.h>
#include <wind_vane/core/wind_vane.h>
#include <wind_vane/interfaces/calibration_storage.h>
#include <wind_vane/interfaces/user_interface.h>
```

#### 3.2 Update Type Usage
**Using backward compatibility aliases:**
```cpp
// This works immediately with new files
IADC* adc = new ESP32AnalogDriver(pin);  // Uses alias
WindVaneMenu* menu = new MainMenu(...);  // Uses alias
```

**Migrating to new names:**
```cpp
// Gradual migration to clear names
AnalogToDigitalConverter* adc = new ESP32AnalogDriver(pin);
MainMenu* menu = new MainMenu(...);
```

## File Mapping Reference

### Core Interfaces
| Old File | New File | Status |
|----------|----------|---------|
| `IADC.h` | `interfaces/analog_to_digital_converter.h` | ✅ Created |
| `IIO.h` | `interfaces/user_interface.h` | ✅ Created |
| `ICalibrationStorage.h` | `interfaces/calibration_storage.h` | 📝 Next |
| `IDiagnostics.h` | `interfaces/diagnostics.h` | 📝 Next |

### Hardware Drivers
| Old File | New File | Status |
|----------|----------|---------|
| `Drivers/ESP32/ADC.h` | `drivers/esp32/esp32_adc_driver.h` | ✅ Created |
| `host/NullADC.h` | `drivers/host/mock_adc_driver.h` | 📝 Next |

### User Interface
| Old File | New File | Status |
|----------|----------|---------|
| `WindVaneMenu.h` | `ui/main_menu.h` | ✅ Created |
| `DiagnosticsMenu.h` | `ui/diagnostics_display.h` | 📝 Next |
| `SettingsMenu.h` | `ui/settings_menu.h` | 📝 Next |

### Storage Components
| Old File | New File | Status |
|----------|----------|---------|
| `EEPROMCalibrationStorage.h` | `storage/eeprom_calibration_storage.h` | 📝 Next |
| `FileCalibrationStorage.h` | `storage/file_calibration_storage.h` | 📝 Next |

### Tests
| Old File | New File | Status |
|----------|----------|---------|
| `test_calibration/ClusterManager.cpp` | `test/unit/core/calibration/cluster_manager_test.cpp` | ✅ Created |
| `test_io/IOHandlers.cpp` | `test/unit/ui/io_handlers_test.cpp` | 📝 Next |

## Implementation Scripts

### Script 1: Create Symbolic Links (Temporary Bridge)
```bash
#!/bin/bash
# Create symbolic links for gradual migration

# Create links for major interfaces
ln -s ../wind_vane/interfaces/analog_to_digital_converter.h lib/WindVane/IADC_new.h
ln -s ../wind_vane/interfaces/user_interface.h lib/WindVane/UI/IIO_new.h

# Create links for implementations
ln -s ../../wind_vane/drivers/esp32/esp32_adc_driver.h lib/WindVane/Drivers/ESP32/ADC_new.h
```

### Script 2: Update Include Paths
```bash
#!/bin/bash
# Script to update include statements gradually

# Function to replace includes in a file
update_includes() {
    local file=$1
    
    # Replace old includes with new ones
    sed -i 's|#include <IADC.h>|#include <wind_vane/interfaces/analog_to_digital_converter.h>|g' "$file"
    sed -i 's|#include <UI/IIO.h>|#include <wind_vane/interfaces/user_interface.h>|g' "$file"
    sed -i 's|#include <WindVane.h>|#include <wind_vane/core/wind_vane.h>|g' "$file"
}

# Update all cpp and h files
find lib src -name "*.cpp" -o -name "*.h" | while read file; do
    echo "Updating includes in $file"
    update_includes "$file"
done
```

### Script 3: Validate Migration
```bash
#!/bin/bash
# Validate that migration doesn't break builds

echo "Building with new structure..."
mkdir -p build_new
cd build_new
cmake ..
make

if [ $? -eq 0 ]; then
    echo "✅ New structure builds successfully"
else
    echo "❌ Build failed with new structure"
    exit 1
fi
```

## Testing Strategy

### 1. Compatibility Tests
```cpp
// Test that aliases work correctly
TEST(MigrationTest, BackwardCompatibilityAliases) {
    // Old style should still work
    IADC* adc = new ESP32AnalogDriver(1);
    EXPECT_NE(adc, nullptr);
    
    // New style should also work
    AnalogToDigitalConverter* adc2 = new ESP32AnalogDriver(1);
    EXPECT_NE(adc2, nullptr);
    
    // They should be the same type
    EXPECT_EQ(typeid(*adc), typeid(*adc2));
}
```

### 2. Include Path Tests
```cpp
// Verify new include paths work
#include <wind_vane/interfaces/analog_to_digital_converter.h>
#include <wind_vane/drivers/esp32/esp32_adc_driver.h>

TEST(MigrationTest, NewIncludePathsWork) {
    ESP32AnalogDriver driver(1);
    EXPECT_TRUE(driver.isAvailable());
}
```

## Rollback Plan

If issues arise during migration:

1. **Immediate Rollback**: Use git to revert to previous working state
2. **Selective Rollback**: Remove new directory structure, keep old files
3. **Hybrid Approach**: Keep both structures until migration is complete

## Benefits After Migration

1. **Clearer Code Organization**: Files are logically grouped
2. **Better IDE Support**: Improved auto-completion and navigation
3. **Easier Onboarding**: New developers can understand structure quickly
4. **Reduced Build Times**: More focused include paths
5. **Better Testing**: Clear separation of unit/integration tests

## Completion Checklist

- [ ] Phase 1: New structure created
- [ ] Phase 2: Build system updated
- [ ] Phase 3: Includes migrated
- [ ] Phase 4: All tests passing
- [ ] Phase 5: Documentation updated
- [ ] Phase 6: Legacy files removed

## Timeline Estimate

- **Week 1-2**: Create remaining new files and aliases
- **Week 3-4**: Update build system and test compilation
- **Week 5-6**: Gradual migration of includes and usage
- **Week 7-8**: Testing and validation
- **Week 9-10**: Legacy cleanup and documentation

This migration can be done incrementally without breaking existing functionality.