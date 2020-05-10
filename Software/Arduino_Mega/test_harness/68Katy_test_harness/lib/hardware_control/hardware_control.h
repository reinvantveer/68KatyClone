#include <address_bus_control.h>
#include <data_bus_control.h>

#define HARDWARE_CONTROL 1

// Assign the Data Acknowledge pin
#define DTACK 27

// Assign the reset circuit pin
#define RESET 29

// Set the serial communication baudrate
#define BAUDRATE 115200


// Sends a single DTACK signal to advance the chip through its instructions
void dtack_pulse() {
  // Set DTACK low to active
  digitalWrite(DTACK, LOW);
  // Wait for a split second, 5 microseconds appears to be the minimum using a 100nF cap
  delayMicroseconds(5);
  digitalWrite(DTACK, HIGH);
  // Wait for a split second
  delayMicroseconds(5);
}

// Set up the data acknowledge signal
void dtack_setup() {
  pinMode(DTACK, OUTPUT);
  // DTACK is an active-low signal, so we're keeping it high
  digitalWrite(DTACK, HIGH);
}


// Gloss over the first 8 data bus read cycles to set dummy values for the stack pointer and program counter of the chip
void init_m68k() {
  Serial.println("Booting: setting dummy stack pointer and program counter in 8 cycles.");
  for (unsigned long address_check = 0; address_check < 8; address_check++) {
    dtack_pulse();
  }
}


// Reset the 68k chip
void reset_setup() {
  pinMode(RESET, OUTPUT);
  // RESET is an active-low signal, so we're pulling it low
  digitalWrite(RESET, LOW);
  // The reset needs some time to be active
  delay(1);
  // Then we're keeping it high, untill we reset again
  digitalWrite(RESET, HIGH);
  delay(1000);
  init_m68k();
}

