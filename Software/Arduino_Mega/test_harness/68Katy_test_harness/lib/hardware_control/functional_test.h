#include <Arduino.h>
#include "hardware_control.h"

// Runs a free-running loop over the entire address space of the Motorola 68k
void freerunTest() {
  Serial.println("Checking for correct start address from fresh reset.");
  unsigned long address = read_address_bus();

  if (address > 0) {
    Serial.print("Error: initial address " + String(address) + " was expected to be 0. There must be something wrong with the reset circuit");
    return;
  }
  Serial.println("Start address OK: " + String(address));

  // Gloss over the first 8 data bus read cycles to account for the start and reset vectors of the chip
  Serial.println("Booting: iterating over start and reset vectors in 1024 + 8 cycles.");
  for (unsigned long address_check = 0; address_check < 8; address_check++) {
    dtack_pulse();
  }

  // After 8 data bus cycles, the actual freerunning can begin
  // We're jumping the odd addresses since our NOP operation takes two data bus cycles
  unsigned long address_space = 1ul << ADDRESS_BUS_SIZE; // pow(2, 20); is slower
  Serial.println("Iterating over address space of size " + String(address_space));

  for (unsigned long address_check = 0; address_check < address_space; address_check += 2) {
    address = read_address_bus();

    if (address_check & 0x0FFF) {
      // Do nothing
    } else {
      Serial.println("Checking address " + String(address_check));

      // Validate that we're advancing two addresses at a time
      // If not: print the encountered address discrepancy and return
      if (address_check != address) {
        Serial.println("Error: expected address: " + String(address_check) + " got " + String(address));

        // Print the contents of the address bus
        Serial.print("Address binary: ");
        for (int pin = ADDRESS_BUS_SIZE - 1; pin >= 0; pin--) {
          unsigned int bit = digitalRead(address_pins[pin]);
          Serial.print(String(bit));
          // Serial.println("Address line " + String(address_lines[pin]) + " is " + String(bit));
        }
        Serial.print("\n");

        // Prematurely end the free-running function
        return;
      }

    }


    // NOP high byte
    data_write(0x004E);
    dtack_pulse();

    // NOP low byte
    data_write(0x0071);
    dtack_pulse();
  }

  Serial.println("Freerunning test OK!");
}
