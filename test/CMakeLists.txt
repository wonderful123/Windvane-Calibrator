cmake_minimum_required(VERSION 3.16)
project(WindVaneTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GoogleTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${GTEST_INCLUDE_DIRS}
)

# Source directories
set(WINDVANE_SRC_DIR ${CMAKE_SOURCE_DIR}/src/WindVane)
set(TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Collect all WindVane source files
file(GLOB_RECURSE WINDVANE_SOURCES 
    "${WINDVANE_SRC_DIR}/*.cpp"
    "${WINDVANE_SRC_DIR}/*.cc"
)

# Unit test sources
set(UNIT_TEST_SOURCES
    unit/test_wind_vane_core.cpp
    unit/test_calibration_manager.cpp
    unit/test_menu_system.cpp
    unit/test_storage_system.cpp
)

# Integration test sources
set(INTEGRATION_TEST_SOURCES
    integration/test_complete_system.cpp
)

# Create unit test executable
add_executable(windvane_unit_tests ${UNIT_TEST_SOURCES} ${WINDVANE_SOURCES})
target_link_libraries(windvane_unit_tests 
    ${GTEST_LIBRARIES} 
    ${GTEST_MAIN_LIBRARIES}
    Threads::Threads
)

# Create integration test executable
add_executable(windvane_integration_tests ${INTEGRATION_TEST_SOURCES} ${WINDVANE_SOURCES})
target_link_libraries(windvane_integration_tests 
    ${GTEST_LIBRARIES} 
    ${GTEST_MAIN_LIBRARIES}
    Threads::Threads
)

# Enable testing
enable_testing()

# Add unit tests
add_test(NAME WindVaneUnitTests COMMAND windvane_unit_tests)
add_test(NAME WindVaneIntegrationTests COMMAND windvane_integration_tests)

# Set test properties
set_tests_properties(WindVaneUnitTests PROPERTIES
    TIMEOUT 300
    ENVIRONMENT "WINDVANE_TEST_MODE=1"
)

set_tests_properties(WindVaneIntegrationTests PROPERTIES
    TIMEOUT 600
    ENVIRONMENT "WINDVANE_TEST_MODE=1"
)

# Compiler flags
target_compile_options(windvane_unit_tests PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
    -g
)

target_compile_options(windvane_integration_tests PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
    -g
)

# Platform-specific settings
if(UNIX)
    target_link_libraries(windvane_unit_tests pthread)
    target_link_libraries(windvane_integration_tests pthread)
endif()

# Print configuration info
message(STATUS "WindVane Test Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Unit Tests: ${UNIT_TEST_SOURCES}")
message(STATUS "  Integration Tests: ${INTEGRATION_TEST_SOURCES}")
message(STATUS "  WindVane Sources: ${WINDVANE_SOURCES}")