#include <Arduino.h>

//Mapping for data pins
#define DATA_BUS_SIZE 8
const String data_lines[DATA_BUS_SIZE] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"};
const unsigned int data_pins[DATA_BUS_SIZE] =  {31,   33,   35,   37,   39,   41,  43,  45};


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


// Writes data to the data bus
void write_data_bus(unsigned int data) {
  // Serial.println("Data: " + String(data));
  for (unsigned int pin = 0; pin < DATA_BUS_SIZE ; pin++) {
    bool masked = data & 1 << pin;
    digitalWrite(data_pins[pin], masked);
  }
}
