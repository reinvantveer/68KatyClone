#include <Arduino.h>

#ifndef DATA_BUS_CONTROL
#include <data_bus_control.h>
#endif

// Set the serial communication baudrate
#define BAUDRATE 115200


struct Mega2M68k8
{
  int last_data_bus_contents;

  Mega2M68k8(){
    // Set up the serial communication with the host machine interfacing with Arduino
    Serial.begin(BAUDRATE);

  }

  void print_last_data_bus_byte(){
    Serial.println("The contents of the data bus at the last data strobe event was " + String(last_data_bus_contents));
  }

};
