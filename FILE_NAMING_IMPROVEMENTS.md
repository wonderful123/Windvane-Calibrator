# File Naming Analysis and Improvements

## Current Naming Issues

### 1. **Interface Naming Inconsistency**
**Problems:**
- Mix of `I` prefix and descriptive names
- Inconsistent interface file organization

**Current:**
```
IADC.h                           // I prefix
ICalibrationStorage.h           // I prefix
IInputOutput.h                  // I prefix but new file
IIO.h                          // I prefix + abbreviation
ConsoleIOHandler.h             // No I prefix for interface implementation
SerialIOHandler.h              // No I prefix for interface implementation
```

**Improved Structure:**
```
interfaces/
├── analog_to_digital_converter.h     // Clear, descriptive name
├── calibration_storage.h             // Interface, no I prefix needed
├── user_input_output.h               // Clear interface name
├── diagnostics.h                     // Simple, clear
└── platform.h                       // Platform abstraction

implementations/
├── esp32_adc.h                       // Platform-specific implementation
├── console_io_handler.h              // Implementation class
├── serial_io_handler.h               // Implementation class
└── file_calibration_storage.h       // Implementation class
```

### 2. **Directory Structure Issues**

**Current Problems:**
```
lib/WindVane/                  // Good top-level
  ├── UI/                     // UI is too generic
  ├── Drivers/ESP32/          // Platform-specific mixed with generic
  ├── Storage/Settings/       // Nested unnecessarily deep
  └── Calibration/Strategies/ // Good structure
```

**Improved Structure:**
```
lib/wind_vane/                 // Snake case for directories
  ├── interfaces/              // All interfaces together
  ├── core/                   // Core business logic
  │   ├── calibration/
  │   └── measurement/
  ├── storage/                // Simplified storage
  ├── ui/                     // User interface components
  ├── drivers/                // Hardware drivers
  │   ├── esp32/
  │   └── host/               // Host/test implementations
  └── services/               // Application services
```

### 3. **File Naming Conventions**

**Issues:**
- Mixed PascalCase and snake_case
- Abbreviations vs full names
- Redundant naming

**Current Examples:**
```
ADC.h                          // Abbreviation
IADC.h                         // Interface + abbreviation
EEPROMCalibrationStorage.h     // Acronym + descriptive
WindVaneMenu.h                 // Redundant "WindVane" prefix
MenuDisplayController.h        // Long, unclear
```

**Improved Naming:**
```
analog_to_digital_converter.h  // Full descriptive name
esp32_adc_driver.h             // Platform + component + type
eeprom_storage.h               // Simplified, clear
main_menu.h                    // Simple, descriptive
display_controller.h           // Role-based naming
```

## Recommended File Reorganization

### Core Interfaces
```
lib/wind_vane/interfaces/
├── analog_to_digital_converter.h
├── calibration_storage.h
├── settings_storage.h
├── user_interface.h
├── diagnostics.h
├── platform.h
└── calibration_strategy.h
```

### Core Business Logic
```
lib/wind_vane/core/
├── wind_vane.h
├── wind_vane.cpp
├── calibration/
│   ├── calibration_manager.h
│   ├── calibration_manager.cpp
│   ├── cluster_manager.h
│   ├── cluster_manager.cpp
│   └── strategies/
│       ├── spinning_calibration.h
│       └── spinning_calibration.cpp
└── measurement/
    ├── direction_calculator.h
    └── direction_calculator.cpp
```

### Platform Drivers
```
lib/wind_vane/drivers/
├── esp32/
│   ├── esp32_adc_driver.h
│   ├── esp32_adc_driver.cpp
│   ├── esp32_serial_interface.h
│   └── esp32_eeprom_storage.h
└── host/
    ├── mock_adc_driver.h
    ├── console_interface.h
    └── file_storage.h
```

### Storage Components
```
lib/wind_vane/storage/
├── file_calibration_storage.h
├── file_calibration_storage.cpp
├── eeprom_calibration_storage.h
├── eeprom_calibration_storage.cpp
├── settings_manager.h
└── settings_manager.cpp
```

### User Interface
```
lib/wind_vane/ui/
├── menu_controller.h
├── menu_controller.cpp
├── diagnostics_display.h
├── diagnostics_display.cpp
└── actions/
    ├── calibrate_action.h
    ├── display_action.h
    └── settings_action.h
```

### Application Services
```
lib/wind_vane/services/
├── dependency_container.h
├── wind_vane_builder.h
├── application_service.h
└── diagnostics_service.h
```

## Test File Organization

**Current Issues:**
```
test/test_calibration/ClusterManager.cpp    // Inconsistent naming
test/test_io/IOHandlers.cpp                 // Generic naming
```

**Improved Structure:**
```
test/
├── unit/
│   ├── core/
│   │   ├── wind_vane_test.cpp
│   │   └── calibration/
│   │       ├── cluster_manager_test.cpp
│   │       └── spinning_calibration_test.cpp
│   ├── storage/
│   │   ├── file_storage_test.cpp
│   │   └── eeprom_storage_test.cpp
│   └── drivers/
│       ├── esp32_adc_test.cpp
│       └── mock_adc_test.cpp
├── integration/
│   ├── calibration_workflow_test.cpp
│   └── menu_navigation_test.cpp
└── fixtures/
    ├── test_data.h
    └── mock_implementations.h
```

## Implementation Strategy

### Phase 1: New File Structure (No Breaking Changes)
1. Create new directory structure alongside existing
2. Create improved interfaces with clear names
3. Add adapter classes for backward compatibility

### Phase 2: Gradual Migration
1. Move new components to improved structure
2. Update build system to include new paths
3. Create symbolic links for transition period

### Phase 3: Legacy Cleanup
1. Remove old file structure
2. Update all includes
3. Remove adapter classes

## Naming Convention Rules

### Files and Directories
- Use `snake_case` for all file and directory names
- Use descriptive, full words instead of abbreviations
- Avoid redundant prefixes (e.g., don't prefix everything with "WindVane")

### Classes and Interfaces
- Use `PascalCase` for class names
- Interfaces don't need `I` prefix if context is clear
- Use descriptive names that indicate purpose

### Example Transformations
```
Before                          After
------                          -----
IADC.h                         → interfaces/analog_to_digital_converter.h
EEPROMCalibrationStorage.h     → storage/eeprom_calibration_storage.h
WindVaneMenu.h                 → ui/main_menu.h
MenuDisplayController.h        → ui/display_controller.h
DiagnosticsMenu.cpp            → ui/diagnostics_display.cpp
test_io/IOHandlers.cpp         → test/unit/ui/io_handlers_test.cpp
```

## Benefits of Improved Naming

1. **Clarity**: No ambiguity about file purpose
2. **Consistency**: Uniform naming convention throughout
3. **Maintainability**: Easier to find and organize code
4. **Onboarding**: New developers can navigate codebase easily
5. **IDE Support**: Better auto-completion and navigation
6. **Build System**: Cleaner include paths and dependencies