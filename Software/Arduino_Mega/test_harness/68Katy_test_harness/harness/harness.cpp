#include <Arduino.h>
#include "freerun_test.h"
#include "rom_test.h"

/*
A test harness for checking chip and board functionality for 68k projects.
Uses an Arduino Mega 2560 for testing. 

You can either use this file from PlatformIO (https://platformio.org) or simple Arduino IDE.
But I much prefer PlatformIO for better IDE functionality, faster compilation, autocomplete etc. 

*/


void setup() {
  Serial.begin(BAUDRATE);

  // Set up the pin states
  address_pins_as_inputs();
  data_pins_as_outputs();
  data_write(0);

  // Set up the DTACK pin
  dtack_setup();

  // Reset the chip to get a fresh readout
  reset_setup();

  // Allow the chip to reset
  delay(1000);

  // Start testing the free-running capability of the m68k
  freerun_test();

  // Test the ROM functionality
  rom_test();
}

void loop() {
  // For performance measurement
  dtack_pulse(); // 0.02 ms
  read_address_bus(); // 0.109 ms
}
