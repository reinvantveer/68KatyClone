#include <Arduino.h>

// Pin mapping for address lines
#define ADDRESS_BUS_SIZE 20
const String address_lines[ADDRESS_BUS_SIZE] = {
    "A0",  "A1",  "A2",  "A3",  "A4",  "A5",  "A6",  "A7",  "A8",  "A9",
    "A10", "A11", "A12", "A13", "A14", "A15", "A16", "A17", "A18", "A19"
};
// These lines correspond with the following Arduino pins:
const unsigned int address_pins[ADDRESS_BUS_SIZE] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 53, 51, 49, 47};


// Print debug info on address pins mapping
void print_address_pin_mapping() {
    for (int n = 0; n < ADDRESS_BUS_SIZE; ++n) {
      Serial.println(
        "Address line " + address_lines[n] + " mapped to Arduino pin " + String(int(address_pins[n]))
      );
  }
}


// Sets the address pins as inputs
void address_pins_as_inputs() {
    for (int n = 0; n < ADDRESS_BUS_SIZE; ++n) {
      pinMode(address_pins[n], INPUT);
      Serial.println(
        "Address line " + address_lines[n] + " set as input to Arduino pin " + String(int(address_pins[n]))
      );
  }
}


// Sets the address pins as outputs
void address_pins_as_outputs() {
    for (int n = 0; n < ADDRESS_BUS_SIZE; ++n) {
      pinMode(address_pins[n], OUTPUT);
      Serial.println("Set address bus as output");
  }
}


// Reads the address bus for the requested address and returns it as a number
unsigned long read_address_bus() {
  unsigned long address = 0;

  // The offset is doubled each time we switch to the next pin
  unsigned long offset = 1;
  unsigned int bit;

  for (int pin = 0; pin < ADDRESS_BUS_SIZE; ++pin) {
    bit = digitalRead(address_pins[pin]);
    if (bit) {
      // Increase the address only if the address pin is 1
      address += offset;
    }
    
    // Inrease the offset by factor 2
    offset <<= 1;
  }

  return address;
}


void write_address_bus(unsigned long address) {

}