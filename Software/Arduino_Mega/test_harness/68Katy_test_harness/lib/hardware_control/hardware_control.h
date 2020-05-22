#include <address_bus_control.h>

#ifndef DATA_BUS_CONTROL
#include <data_bus_control.h>
#endif

#define HARDWARE_CONTROL 1

// Assign the Data Acknowledge pin
#define DTACK 27

// Assign the reset circuit pin
#define RESET 29



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
  Serial.println("Booting: setting stack pointer and program counter in 8 cycles.");

  data_pins_as_outputs();

  // BRA $0000
  // 0x60 0x00 (16 bit displacement) 0x0000 (16 bit displacement of 0)
  write_data_bus(0x60);
  dtack_pulse();

  for (unsigned long address_check = 0; address_check < 7; address_check++) {
    write_data_bus(0x00);
    dtack_pulse();
  }
}


// Reset the 68k chip
void reset_setup() {
  pinMode(RESET, OUTPUT);
  // RESET is an active-low signal, so we're pulling it low
  digitalWrite(RESET, LOW);
  // The reset needs some time to be active
  delayMicroseconds(30);
  // Then we're keeping it high, until we reset again
  digitalWrite(RESET, HIGH);
  delay(1000);
  init_m68k();
}

// Following https://forum.arduino.cc/index.php?topic=49581.msg354263#msg354263
void arduino_reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  asm volatile ("  jmp 0");  
}  