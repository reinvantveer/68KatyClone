#include <Arduino.h>

// Pin mapping for address lines
#define ADDRESS_BUS_SIZE 20
const String address_lines[ADDRESS_BUS_SIZE] = {
    "A0",  "A1",  "A2",  "A3",  "A4",  "A5",  "A6",  "A7",  "A8",  "A9",
    "A10", "A11", "A12", "A13", "A14", "A15", "A16", "A17", "A18", "A19"
};
// These lines correspond with the following Arduino pins:
const unsigned int address_pins[ADDRESS_BUS_SIZE] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 53, 51, 49, 47};

//Mapping for data pins
#define DATA_BUS_SIZE 8
const String data_lines[DATA_BUS_SIZE] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"};
const unsigned int data_pins[DATA_BUS_SIZE] =  {31,   33,   35,   37,   39,   41,  43,  45};

// Assign the Data Acknowledge pin
#define DTACK 27

// Assign the reset circuit pin
#define RESET 29

// Set the serial communication baudrate
#define BAUDRATE 115200


// Sets the address pins as inputs
void address_pins_as_inputs() {
    for (int n = 0; n < ADDRESS_BUS_SIZE; ++n) {
      pinMode(address_pins[n], INPUT);
      Serial.println(
        "Address line " + address_lines[n] + " set as input to Arduino pin " + String(int(address_pins[n]))
      );
  }
}


// Writes data to the data bus
void data_write(unsigned int data) {
  // Serial.println("Data: " + String(data));
  for (unsigned int pin = 0; pin < DATA_BUS_SIZE ; pin++) {
    bool masked = data & 1 << pin;
    // Serial.println("Pin/bit " + String(data_lines[pin]) + " value: " + String(masked));
    digitalWrite(data_pins[pin], masked);
  }
}


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

// Sets the data pins as outputs
void data_pins_as_outputs() {
  for (int n = 0; n < DATA_BUS_SIZE ; ++n) {
    pinMode(data_pins[n], OUTPUT);
    Serial.println(
      "Data line " + data_lines[n] + " set as output to Arduino pin " + String(int(data_pins[n]))
    );
  }
}


// Set up the data acknowledge signal
void dtack_setup() {
  pinMode(DTACK, OUTPUT);
  // DTACK is an active-low signal, so we're keeping it high
  digitalWrite(DTACK, HIGH);
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
