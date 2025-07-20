#!/bin/bash

# WindVane Library Test Runner
# This script builds and runs all tests for the WindVane library

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
BUILD_DIR="build"
TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$TEST_DIR")"

echo -e "${BLUE}=== WindVane Library Test Runner ===${NC}"
echo "Project Root: $PROJECT_ROOT"
echo "Test Directory: $TEST_DIR"
echo "Build Directory: $BUILD_DIR"
echo ""

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "$PROJECT_ROOT/library.json" ]; then
    print_error "Not in WindVane project root. Please run from project root directory."
    exit 1
fi

# Check for required tools
check_requirements() {
    print_status "Checking requirements..."
    
    if ! command -v cmake &> /dev/null; then
        print_error "CMake is required but not installed."
        exit 1
    fi
    
    if ! command -v make &> /dev/null; then
        print_error "Make is required but not installed."
        exit 1
    fi
    
    # Check for GoogleTest
    if ! pkg-config --exists gtest; then
        print_warning "GoogleTest not found via pkg-config. Will try to find via CMake."
    fi
    
    print_status "Requirements check passed."
}

# Clean build directory
clean_build() {
    print_status "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
    mkdir -p "$BUILD_DIR"
}

# Build tests
build_tests() {
    print_status "Building tests..."
    
    cd "$BUILD_DIR"
    
    # Configure with CMake
    cmake -DCMAKE_BUILD_TYPE=Debug "$TEST_DIR"
    
    # Build
    make -j$(nproc)
    
    cd "$PROJECT_ROOT"
    
    print_status "Build completed successfully."
}

# Run unit tests
run_unit_tests() {
    print_status "Running unit tests..."
    
    cd "$BUILD_DIR"
    
    if [ -f "./windvane_unit_tests" ]; then
        ./windvane_unit_tests --gtest_output=xml:unit_test_results.xml
        UNIT_EXIT_CODE=$?
    else
        print_error "Unit test executable not found!"
        UNIT_EXIT_CODE=1
    fi
    
    cd "$PROJECT_ROOT"
    
    if [ $UNIT_EXIT_CODE -eq 0 ]; then
        print_status "Unit tests passed."
    else
        print_error "Unit tests failed!"
    fi
    
    return $UNIT_EXIT_CODE
}

# Run integration tests
run_integration_tests() {
    print_status "Running integration tests..."
    
    cd "$BUILD_DIR"
    
    if [ -f "./windvane_integration_tests" ]; then
        ./windvane_integration_tests --gtest_output=xml:integration_test_results.xml
        INTEGRATION_EXIT_CODE=$?
    else
        print_error "Integration test executable not found!"
        INTEGRATION_EXIT_CODE=1
    fi
    
    cd "$PROJECT_ROOT"
    
    if [ $INTEGRATION_EXIT_CODE -eq 0 ]; then
        print_status "Integration tests passed."
    else
        print_error "Integration tests failed!"
    fi
    
    return $INTEGRATION_EXIT_CODE
}

# Generate test report
generate_report() {
    print_status "Generating test report..."
    
    if [ -f "$BUILD_DIR/unit_test_results.xml" ]; then
        echo "Unit Test Results:"
        echo "=================="
        grep -E "(testsuites|testsuite|testcase)" "$BUILD_DIR/unit_test_results.xml" | head -20
        echo ""
    fi
    
    if [ -f "$BUILD_DIR/integration_test_results.xml" ]; then
        echo "Integration Test Results:"
        echo "========================"
        grep -E "(testsuites|testsuite|testcase)" "$BUILD_DIR/integration_test_results.xml" | head -20
        echo ""
    fi
}

# Main execution
main() {
    local unit_result=0
    local integration_result=0
    
    check_requirements
    clean_build
    build_tests
    
    print_status "Starting test execution..."
    echo ""
    
    # Run unit tests
    run_unit_tests
    unit_result=$?
    
    echo ""
    
    # Run integration tests
    run_integration_tests
    integration_result=$?
    
    echo ""
    
    # Generate report
    generate_report
    
    # Final status
    echo -e "${BLUE}=== Test Summary ===${NC}"
    if [ $unit_result -eq 0 ]; then
        echo -e "${GREEN}✓ Unit Tests: PASSED${NC}"
    else
        echo -e "${RED}✗ Unit Tests: FAILED${NC}"
    fi
    
    if [ $integration_result -eq 0 ]; then
        echo -e "${GREEN}✓ Integration Tests: PASSED${NC}"
    else
        echo -e "${RED}✗ Integration Tests: FAILED${NC}"
    fi
    
    # Overall result
    if [ $unit_result -eq 0 ] && [ $integration_result -eq 0 ]; then
        echo -e "${GREEN}🎉 All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}❌ Some tests failed!${NC}"
        exit 1
    fi
}

# Run main function
main "$@"