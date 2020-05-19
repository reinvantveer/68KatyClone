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

  Serial.println("\nOr: perform some basic operation:");
  Serial.println("\t[3] Reset the 68k chip. The RAM contents are unaffected.");
  Serial.println("\t[4] Read the contents of the address bus");
  Serial.println("\t[5] Read the contents of the data bus");
  Serial.println("\t[6] Acknowledge one data bus cycle (DTACK)");
}

void setup() {
  // Set up the serial communication with the host machine interfacing with Arduino
  Serial.begin(BAUDRATE);

  // Set up the DTACK pin
  dtack_setup();

  // We're not going to use the address bus for anything else but inputs
  address_pins_as_inputs();
  data_pins_as_inputs();

  // Reset the chip to get a fresh readout
  reset_setup();

  // Show info
  print_address_pin_mapping();
  print_data_pin_mapping();
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

    // Tests
    if (chosen_option == '1') freerun_test();  // Start testing the free-running capability of the m68k
    if (chosen_option == '2') rom_test();      // Test the ROM functionality

    // Basic operation
    if (chosen_option == '3') reset_setup();   // Reset the chip
    if (chosen_option == '4') Serial.println("Address " + String(read_address_bus())); // Print the current address on the address bus
    if (chosen_option == '5') Serial.println("Data: " + String(read_data_bus())); // Print the current data byte on the data bus

    // Afterwards: print the menu again
    print_menu();
  }
}