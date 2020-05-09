#include <Arduino.h>
#include "freerun_test.h"
#include "rom_test.h"

/*
A test harness for checking chip and board functionality for 68k projects.
Uses an Arduino Mega 2560 for testing. 

You can either use this file from PlatformIO (https://platformio.org) or simple Arduino IDE.
But I much prefer PlatformIO for better IDE functionality, faster compilation, autocomplete etc. 

*/
void print_menu() {
    // Show the selection menu
  Serial.println("\nThe board is ready for testing. Please select one of the following tests:");
  Serial.println("\t[1] Freerunning test");
  Serial.println("\t[2] ROM test");
}

void setup() {
  // Set up the serial communication with the host machine interfacing with Arduino
  Serial.begin(BAUDRATE);

  // Set up the DTACK pin
  dtack_setup();

  // Reset the chip to get a fresh readout
  reset_setup();

  // Allow the chip to reset
  delay(1000);

  print_menu();
}

void loop() {
  int incomingByte = 0; // for incoming serial data

  // Only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    char chosen_option = (char)incomingByte;
    Serial.print("Chosen option: ");
    Serial.println(chosen_option);

    if (chosen_option == '1') freerun_test();  // Start testing the free-running capability of the m68k
    if (chosen_option == '2') rom_test();      // Test the ROM functionality
    print_menu();
  }
}