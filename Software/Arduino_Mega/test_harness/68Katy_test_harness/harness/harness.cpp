#include <Arduino.h>
#include "functional_test.h"

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

  delay(1000);
  // Start testing the free-running capability of the m68k
  freerunTest();
}

void loop() {
  // put your main code here, to run repeatedly:
  dtack_pulse();
}
