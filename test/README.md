# WindVane Library Test Suite

Comprehensive testing framework for the WindVane library with unit tests, integration tests, and automated test execution.

## 🧪 Test Structure

```
test/
├── README.md                    # This documentation
├── CMakeLists.txt              # CMake build configuration
├── run_tests.sh                # Automated test runner script
├── unit/                       # Unit tests
│   ├── test_wind_vane_core.cpp
│   ├── test_calibration_manager.cpp
│   ├── test_menu_system.cpp
│   └── test_storage_system.cpp
├── integration/                # Integration tests
│   └── test_complete_system.cpp
└── mocks/                      # Mock objects (future)
```

## 🎯 Test Categories

### **Unit Tests** (`test/unit/`)
- **Core WindVane Tests**: Test the main WindVane class functionality
- **Calibration Manager Tests**: Test calibration system components
- **Menu System Tests**: Test interactive menu functionality
- **Storage System Tests**: Test data persistence components

### **Integration Tests** (`test/integration/`)
- **Complete System Tests**: Test all components working together
- **End-to-End Workflows**: Test complete user scenarios
- **Performance Tests**: Test system performance and memory usage
- **Error Handling Tests**: Test system robustness

## 🚀 Quick Start

### **Prerequisites**
- CMake 3.16 or higher
- Make or Ninja build system
- GoogleTest library
- C++17 compatible compiler

### **Install Dependencies**

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install cmake build-essential libgtest-dev
```

#### macOS:
```bash
brew install cmake googletest
```

#### Windows:
```bash
# Install via vcpkg or download GoogleTest manually
```

### **Run All Tests**
```bash
# From project root directory
./test/run_tests.sh
```

### **Manual Build and Test**
```bash
# Create build directory
mkdir -p test/build
cd test/build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Run unit tests
./windvane_unit_tests

# Run integration tests
./windvane_integration_tests
```

## 📋 Test Coverage

### **Core WindVane Class**
- ✅ Constructor with valid configuration
- ✅ Direction reading with and without calibration
- ✅ Raw reading functionality
- ✅ Calibration status checking
- ✅ Calibration clearing
- ✅ Calibration starting
- ✅ Timestamp retrieval

### **Calibration Manager**
- ✅ Constructor creation
- ✅ Calibration strategy management
- ✅ Progress tracking
- ✅ Completion detection
- ✅ Result retrieval
- ✅ Cancellation handling

### **Menu System**
- ✅ Menu instance creation
- ✅ Main menu display
- ✅ Input handling for all options
- ✅ Direction display functionality
- ✅ Calibration menu integration
- ✅ Diagnostics menu display
- ✅ Settings menu display
- ✅ Help menu display
- ✅ Menu loop operation

### **Storage System**
- ✅ EEPROM calibration storage
- ✅ File-based calibration storage
- ✅ Settings management
- ✅ Data integrity verification
- ✅ Error handling for corrupted data
- ✅ Default settings management
- ✅ Settings reset functionality

### **Integration Tests**
- ✅ Complete system initialization
- ✅ Component interaction
- ✅ End-to-end workflows
- ✅ Performance benchmarks
- ✅ Memory usage validation
- ✅ Error handling scenarios

## 🔧 Test Configuration

### **Build Configuration**
- **C++ Standard**: C++17
- **Build Type**: Debug (for testing)
- **Compiler Flags**: -Wall -Wextra -Wpedantic
- **Optimization**: -O2 -g

### **Test Environment**
- **Timeout**: 300s for unit tests, 600s for integration tests
- **Environment Variables**: WINDVANE_TEST_MODE=1
- **Output Format**: XML for CI/CD integration

### **Mock Objects**
The test suite uses Google Mock for creating mock objects:
- **MockADC**: Simulates analog-to-digital converter
- **MockCalibrationStorage**: Simulates storage system
- **MockUserIO**: Simulates user interface
- **MockDiagnostics**: Simulates logging system

## 📊 Test Results

### **Expected Output**
```
=== WindVane Library Test Runner ===
[INFO] Checking requirements...
[INFO] Requirements check passed.
[INFO] Cleaning build directory...
[INFO] Building tests...
[INFO] Build completed successfully.
[INFO] Starting test execution...

[INFO] Running unit tests...
[==========] Running 25 tests from 4 test suites.
[----------] Global test environment set-up.
[----------] 25 tests from 4 test suites
[----------] Global test environment tear-down.
[==========] 25 tests from 4 test suites ran. (15 ms total)
[INFO] Unit tests passed.

[INFO] Running integration tests...
[==========] Running 12 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 12 tests from 1 test suite
[----------] Global test environment tear-down.
[==========] 12 tests from 1 test suite ran. (45 ms total)
[INFO] Integration tests passed.

=== Test Summary ===
✓ Unit Tests: PASSED
✓ Integration Tests: PASSED
🎉 All tests passed!
```

### **Test Metrics**
- **Unit Tests**: 25 tests across 4 test suites
- **Integration Tests**: 12 tests across 1 test suite
- **Total Coverage**: Core functionality, calibration, menu, storage
- **Performance**: <100ms response time for core operations
- **Memory**: Reasonable footprint with multiple instances

## 🐛 Debugging Tests

### **Running Individual Tests**
```bash
# Run specific test suite
./windvane_unit_tests --gtest_filter=WindVaneCoreTest*

# Run specific test
./windvane_unit_tests --gtest_filter=WindVaneCoreTest.GetDirection_WithCalibration_ReturnsCalibratedValue

# Run with verbose output
./windvane_unit_tests --gtest_verbose
```

### **Debugging Failed Tests**
```bash
# Run with debug output
./windvane_unit_tests --gtest_break_on_failure

# Generate detailed XML report
./windvane_unit_tests --gtest_output=xml:detailed_results.xml
```

### **Common Issues**

#### **GoogleTest Not Found**
```bash
# Install GoogleTest
sudo apt install libgtest-dev
# Or build from source
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
sudo make install
```

#### **Compilation Errors**
- Ensure C++17 is supported by your compiler
- Check that all WindVane headers are in the include path
- Verify that all source files are being compiled

#### **Test Failures**
- Check that mock objects are properly configured
- Verify that test data is valid
- Ensure that hardware dependencies are available for integration tests

## 🔄 Continuous Integration

### **GitHub Actions Example**
```yaml
name: WindVane Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: |
        sudo apt update
        sudo apt install cmake build-essential libgtest-dev
    - name: Run tests
      run: ./test/run_tests.sh
```

### **Travis CI Example**
```yaml
language: cpp
compiler: gcc
addons:
  apt:
    packages:
    - cmake
    - libgtest-dev
script:
  - ./test/run_tests.sh
```

## 📈 Adding New Tests

### **Unit Test Template**
```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindVane/Component/Component.h"

using namespace testing;

class ComponentTest : public Test {
protected:
    void SetUp() override {
        // Initialize test fixtures
    }
    
    void TearDown() override {
        // Clean up test fixtures
    }
};

TEST_F(ComponentTest, TestName_ExpectedBehavior_ExpectedResult) {
    // Arrange
    // Act
    // Assert
    EXPECT_EQ(actual, expected);
}
```

### **Integration Test Template**
```cpp
#include <gtest/gtest.h>
#include "WindVane.h"

class IntegrationTest : public Test {
protected:
    void SetUp() override {
        // Initialize real components
    }
};

TEST_F(IntegrationTest, ComponentInteraction_WorksEndToEnd) {
    // Test complete workflows
}
```

## 🎯 Best Practices

### **Test Design**
- **Arrange-Act-Assert**: Structure tests clearly
- **Single Responsibility**: Each test tests one thing
- **Descriptive Names**: Use clear test names
- **Mock Dependencies**: Isolate units under test
- **Test Data**: Use realistic test data

### **Test Maintenance**
- **Keep Tests Fast**: Tests should run quickly
- **Keep Tests Independent**: Tests shouldn't depend on each other
- **Keep Tests Repeatable**: Tests should give same results
- **Keep Tests Self-Validating**: Tests should clearly pass or fail

### **Test Coverage**
- **Happy Path**: Test normal operation
- **Error Paths**: Test error conditions
- **Edge Cases**: Test boundary conditions
- **Integration**: Test component interaction

## 📚 Additional Resources

- **GoogleTest Documentation**: https://google.github.io/googletest/
- **Google Mock Documentation**: https://google.github.io/googletest/gmock_for_dummies.html
- **CMake Testing**: https://cmake.org/cmake/help/latest/command/enable_testing.html

---

**Professional Testing Framework for WindVane Library**  
*Comprehensive, automated, and maintainable test suite*