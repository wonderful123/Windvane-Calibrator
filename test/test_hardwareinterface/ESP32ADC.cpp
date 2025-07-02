#include "../../src/Hardware/ESP32/ADC.h"
#include "../../lib/WindVane/IADC.h"
#include <gtest/gtest.h>

class ESP32ADCTest : public ::testing::Test {
protected:
  std::unique_ptr<IADC> esp32adc;
  const int gpioPin = 1;

  void SetUp() override {
    esp32adc = std::make_unique<ESP32ADC>(gpioPin);
  }
};

TEST_F(ESP32ADCTest, Creation) {
  auto esp32adc_obj = dynamic_cast<ESP32ADC *>(esp32adc.get());
  ASSERT_NE(nullptr, esp32adc_obj);
}

TEST_F(ESP32ADCTest, GPIOPinSet) {
  auto esp32adc_obj = dynamic_cast<ESP32ADC *>(esp32adc.get());
  EXPECT_EQ(esp32adc_obj->getPin(), 1);
}

TEST_F(ESP32ADCTest, ESP32ADC) {
  auto adc = std::make_unique<ESP32ADC>(1);
  auto obj = dynamic_cast<ESP32ADC *>(adc.get());
  ASSERT_NE(nullptr, obj);
}

#if defined(ARDUINO)
#include <Arduino.h>

void setup() {
  // should be the same value as for the `test_speed` option in "platformio.ini"
  // default value is test_speed=115200
  Serial.begin(115200);

  ::testing::InitGoogleTest();
}

void loop() {
  // Run tests
  if (RUN_ALL_TESTS())
    ;

  // sleep 1 sec
  delay(1000);
}

#else

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;
  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
#endif
