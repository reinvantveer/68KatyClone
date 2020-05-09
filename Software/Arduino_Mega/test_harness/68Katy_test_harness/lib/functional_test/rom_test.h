#include <Arduino.h>

#ifndef HARDWARE_CONTROL
#include <hardware_control.h>
#endif

unsigned long hash_rom_space() {
  /*
  The ROM test goes through the address space of the ROM chip with 4096 bit jumps.
  Each time it jumps to the next location, it reads the byte stored there and increases a hash value with the data byte.
  */

  unsigned long rom_address_space_start = 0x00000;
  unsigned long rom_address_space_end = 0x77FFF;
  unsigned int address_increment = 4096;
  
  unsigned long hash_value = 0;

  for (unsigned long address = rom_address_space_start; address <= rom_address_space_end; address += address_increment) {
    write_address_bus(address);
    delay(50);
    unsigned int data = read_data_bus();
    Serial.print("\rAddress " + String(address) + " has data " + String(data));
    hash_value += data;
    delay(50);
  }

  Serial.println();
  return hash_value; // Dummy value for now
}

void rom_test() {
  /*
  The ROM test gathers a hash value for part of the contents of the ROM. 
  The test then repeats this process, to a second hash.
  Once the entire ROM address space is visited twice, the hash values should not be 0 and should be equal.
  */

  // First of all, reset the m68k system
  reset_setup();
  data_pins_as_inputs();
  address_pins_as_outputs();

  unsigned long hash_first_pass = hash_rom_space();
  if (! hash_first_pass) {
    Serial.println("Error! ROM hash for the first pass was 0.");
    return;
  }

  reset_setup();
  unsigned long hash_second_pass = hash_rom_space();
  if (! hash_second_pass) {
    Serial.println("Error! ROM hash for the second pass was 0.");
    return;
  }

  if (!(hash_first_pass == hash_second_pass)) {
    Serial.println("Error! ROM hash of pass 1: " + String(hash_first_pass) + " was not identical to pass 2: " + String(hash_second_pass));
    return;
  }

  Serial.println("ROM test success!");

}