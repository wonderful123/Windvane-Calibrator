# Host Simulation Example

This example demonstrates the WindVane library on desktop platforms with simulated sensor data and file-based storage.

## Features

- **Desktop Simulation**: Run WindVane library on your computer
- **Simulated Sensor Data**: Realistic wind vane simulation
- **File-based Storage**: Calibration data saved to files
- **Console Interface**: Full serial-like interface
- **No Hardware Required**: Test the library without physical sensors

## Usage

### PlatformIO (Recommended)

1. Open the example folder in PlatformIO
2. Select the `native` environment
3. Build and run: `pio run -t upload`
4. Monitor output: `pio device monitor`

### Arduino IDE

1. Open Arduino IDE
2. Open `HostSimulation.ino`
3. Select "Arduino AVR Boards > Arduino Uno" (for native compilation)
4. Upload the sketch
5. Open Serial Monitor at 115200 baud

## Simulation Details

The example simulates a realistic wind vane:

### Simulated Wind Patterns
- **Gradual Changes**: Wind direction changes slowly and realistically
- **Speed Variations**: Wind speed varies over time
- **Realistic Values**: ADC values correspond to actual wind directions

### Calibration Simulation
- **Automatic Calibration**: Simulates rotation through all angles
- **Progress Tracking**: Shows calibration progress
- **Data Validation**: Ensures calibration data is realistic

### Storage Simulation
- **File-based Storage**: Calibration data saved to `calibration.dat`
- **Data Persistence**: Calibration survives program restarts
- **Error Handling**: Simulates storage errors and recovery

## Expected Output

```
WindVane Host Simulation Example
=================================
WindVane initialized successfully
No calibration found, starting automatic calibration...
Starting automatic calibration...
Simulating wind vane rotation...
Calibration progress: 25%
Calibration progress: 50%
Calibration progress: 75%
Calibration progress: 100%
Calibration completed successfully!

Starting wind simulation...
Simulation Step 1:
  Simulated Angle: 45.2°
  Measured Direction: 45°
  Simulated Speed: 8.5 m/s
  Raw ADC Value: 512
  Voltage: 413 mV
  Calibrated: Yes

Simulation Step 2:
  Simulated Angle: 67.8°
  Measured Direction: 68°
  Simulated Speed: 12.3 m/s
  Raw ADC Value: 768
  Voltage: 620 mV
  Calibrated: Yes
...
```

## Configuration

The simulation uses:

- **Vane Type**: Potentiometer (simulated)
- **Calibration Method**: Automatic
- **Storage**: File system (`./calibration.dat`)
- **ADC Resolution**: 12-bit (0-4095)
- **Update Rate**: 50ms for calibration, 1000ms for display

## Use Cases

### Development and Testing
- Test library features without hardware
- Debug calibration algorithms
- Validate storage implementations
- Test error handling

### Documentation and Examples
- Demonstrate library capabilities
- Show expected outputs
- Provide working examples
- Test cross-platform compatibility

### Education and Learning
- Understand wind vane operation
- Learn calibration process
- See how ADC values relate to wind direction
- Explore library architecture

## Troubleshooting

### PlatformIO Issues
- **Build fails**: Ensure native platform is installed
- **No output**: Check that native environment is selected
- **Permission errors**: Run with appropriate permissions

### Arduino IDE Issues
- **No native board**: Install Arduino AVR Boards package
- **Compilation errors**: Check C++17 support
- **No serial output**: Use PlatformIO for better native support

### Simulation Issues
- **Unrealistic values**: Check ADC configuration
- **Calibration fails**: Ensure file write permissions
- **No persistence**: Check file path and permissions

## Advanced Usage

### Customizing Simulation
You can modify the simulation parameters:

```cpp
// Adjust simulation speed
current_angle += (random(-180, 180)) * 0.005f; // Slower changes

// Change ADC range
return static_cast<uint16_t>((current_angle / 360.0f) * 1023.0f); // 10-bit ADC
```

### Adding Real Hardware
To test with real hardware:

1. Replace `WindVaneBuilder::host()` with `WindVaneBuilder::esp32()`
2. Connect actual wind vane sensor
3. Update ADC configuration for your hardware
4. Use appropriate storage backend

### Integration Testing
Use this example for:

- **Unit Testing**: Test individual components
- **Integration Testing**: Test component interactions
- **Performance Testing**: Measure timing and memory usage
- **Regression Testing**: Ensure new features don't break existing functionality

## Next Steps

After running the simulation:

1. **Try Real Hardware**: Use the same code with actual sensors
2. **Customize Simulation**: Modify parameters for your needs
3. **Add More Features**: Extend the simulation capabilities
4. **Integrate with Other Tools**: Connect to data analysis or visualization tools