#include <Arduino.h>
#include "freerun_test.h"
#include "rom_test.h"

/*
A test harness for checking chip and board functionality for 68k projects.
Uses an Arduino Mega 2560 for testing. 

You can either use this file from PlatformIO (https://platformio.org) or simple Arduino IDE.
But I much prefer PlatformIO for better IDE functionality, faster compilation, autocomplete etc. 

*/

#define BAUDRATE 115200

// Flags for keeping track of the input or output mode of the address and data bus pins
bool is_address_bus_in_input_mode;
bool is_data_bus_in_input_mode;

volatile unsigned char last_data_bus_contents;

// Read the data bus when triggered throug a data strobe 
void read_data_bus_triggered(){
  last_data_bus_contents = read_data_bus();
  Serial.println("The data strobe event occurred: data " + String(last_data_bus_contents, HEX));
}

void print_menu() {
    // Show the selection menu
  Serial.println("\nThe board is ready for testing. Please select one of the following tests:");
  Serial.println("\t[1] Freerunning test");
  Serial.println("\t[2] ROM test (Work in progress)");

  Serial.println("\nOr: perform some basic operation:");
  Serial.println("\t[3] Reset the 68k chip. The RAM contents are unaffected.");
  Serial.println("\t[4] Read the contents of the address bus");
  Serial.println("\t[5] Read the contents of the data bus");
  Serial.println("\t[6] Acknowledge one data bus cycle (DTACK)");
  Serial.println();
}

void setup() {
  Serial.begin(BAUDRATE);

  // We're not going to use the address bus for anything else but inputs
  address_pins_as_inputs();
  is_address_bus_in_input_mode = true;
  
  data_pins_as_inputs();
  is_data_bus_in_input_mode = true;
    
  // Set up the DTACK pin
  dtack_setup();

  // Reset the chip to get a fresh readout
  reset_setup();

  // Show info on m68k8 to Arduino pin mappings
  print_address_pin_mapping();
  print_data_pin_mapping();

  print_menu();
  attachInterrupt(digitalPinToInterrupt(DATA_STROBE_PIN), read_data_bus_triggered, RISING);
}

void loop() {
  int incomingByte = 0; // for incoming serial data

  // Only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    char chosen_option = (char)incomingByte;
    // Serial.print("Chosen option: ");
    // Serial.println(chosen_option);

    // Tests
    if (chosen_option == '1') freerun_test();  // Start testing the free-running capability of the m68k
    if (chosen_option == '2') { rom_test(); print_menu(); }     // Test the ROM functionality

    // Basic operation
    if (chosen_option == '3') { reset_setup(); print_menu(); }  // Reset the chip
    if (chosen_option == '4') Serial.print("\r\tAddress: " + String(read_address_bus(), HEX) + "\t\t\t\t\t"); // Print the current address on the address bus
    if (chosen_option == '5') Serial.print("\r\tLast known byte on data bus:" + String(last_data_bus_contents, HEX) + "\t\t\t\t\t"); // Print the last known data byte on the data bus
    if (chosen_option == '6') { dtack_pulse(); Serial.print("\r\tAcknowledged one data bus cycle"); }

    // Afterwards: print the menu again
    // print_menu();
  }
}