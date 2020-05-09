#ifndef ADDRESS_BUS_CONTROL
#include <address_bus_control.h>
#endif

#define HARDWARE_CONTROL 1

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


// Sets the data pins as outputs
void data_pins_as_outputs() {
  for (int n = 0; n < DATA_BUS_SIZE ; ++n) {
    pinMode(data_pins[n], OUTPUT);
    Serial.println(
      "Data line " + data_lines[n] + " set as output to Arduino pin " + String(int(data_pins[n]))
    );
  }
}


// Sets the data pins as outputs
void data_pins_as_inputs() {
  for (int n = 0; n < DATA_BUS_SIZE ; ++n) {
    pinMode(data_pins[n], INPUT);
    Serial.println(
      "Data line " + data_lines[n] + " set as input to Arduino pin " + String(int(data_pins[n]))
    );
  }
}


// Writes data to the data bus
void write_data_bus(unsigned int data) {
  // Serial.println("Data: " + String(data));
  for (unsigned int pin = 0; pin < DATA_BUS_SIZE ; pin++) {
    bool masked = data & 1 << pin;
    digitalWrite(data_pins[pin], masked);
  }
}


// Reads data to the data bus
unsigned int read_data_bus(){
  unsigned int data_bus_byte = 0;
  unsigned long offset = 1;
  unsigned int bit;

  for (unsigned int pin = 0; pin < DATA_BUS_SIZE ; pin++) {
    bit = digitalRead(data_pins[pin]);
    if (bit) {
      // Increase the address only if the address pin is 1
      data_bus_byte += offset;
    }
    
    // Inrease the offset by factor 2
    offset <<= 1;
  }

  return data_bus_byte;
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
