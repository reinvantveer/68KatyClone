#include <Arduino.h>

#define DATA_BUS_CONTROL

// Mapping for data pins
#define DATA_BUS_SIZE 8
const String data_lines[DATA_BUS_SIZE] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"};
// These are the Arduino Mega pin numbers used for the corresponding data lines
const unsigned int data_pins[DATA_BUS_SIZE] =  {31,   33,   35,   37,   39,   41,  43,  45};

// Data strobe pin mapping to Arduino Mega pin number
#define DATA_STROBE_PIN 21


// Print debug info on the data pins mapping
void print_data_pin_mapping() {
  Serial.println("\nData bus pin mapping:");
  for (int n = 0; n < DATA_BUS_SIZE ; ++n) {
    Serial.println(
      "\tData line " + data_lines[n] + " mapped to Arduino pin " + String(int(data_pins[n]))
    );
  }

  Serial.println("\n\tData strobe pin (interruptable): " + String(DATA_STROBE_PIN));
}


// Sets the data pins as outputs
void data_pins_as_outputs() {
  for (int n = 0; n < DATA_BUS_SIZE ; ++n) {
    pinMode(data_pins[n], OUTPUT);
  }
}


// Sets the data pins as outputs
void data_pins_as_inputs() {
  pinMode(DATA_STROBE_PIN, INPUT);

  for (int n = 0; n < DATA_BUS_SIZE ; ++n) {
    pinMode(data_pins[n], INPUT);
  }
}


// Reads data to the data bus
unsigned int read_data_bus(){
  unsigned int data_bus_byte = 0;

  // The offset is doubled each time we switch to the next pin
  unsigned int offset = 1;
  unsigned int bit;

  for (int pin = 0; pin < DATA_BUS_SIZE ; ++pin) {
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


// Writes data to the data bus
void write_data_bus(unsigned int data) {
  // Serial.println("Data: " + String(data));
  for (unsigned int pin = 0; pin < DATA_BUS_SIZE ; pin++) {
    bool masked = data & 1 << pin;
    digitalWrite(data_pins[pin], masked);
  }
}
