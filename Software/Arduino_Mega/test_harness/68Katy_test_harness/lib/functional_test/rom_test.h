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
  // unsigned long rom_address_space_end = 0x77FFF;
  unsigned long rom_address_space_end = 0x5FFFF;

  // This is the space between samples we're taking from the ROM
  unsigned int address_increment = 4096;
  
  // This is going to contain our hash value for the sampled content
  unsigned long hash_value = 0;

  Serial.println("Checking for correct start address from fresh reset.");
  unsigned long address = read_address_bus();

  if (address > 0) {
    Serial.println("Error: initial address " + String(address) + " was expected to be 0. There must be something wrong with the reset circuit or the DTACK is enabled");
    // Abort
    return 0;
  }
  Serial.println("Start address OK: address " + String(address));


  for (unsigned long address = rom_address_space_start; address < rom_address_space_end; address += address_increment) {
    // We checked that the m68k chip is reset at address 0 and we'll assume that the stack pointer and program counter have been initialized 

    // We're going to use this moment to read the contents of the data bus for the low byte
    data_pins_as_inputs();
    delay(100);
    unsigned int data_high = read_data_bus();
    Serial.println("Data high: " + String(data_high));

    // We're going to replace the contents of the data bus with instructions for the chip to jump to the required address
    data_pins_as_outputs();
    delay(100);
    write_data_bus(0x4E); // Basically the JMP opcode
    delay(10);
    dtack_pulse();
    delay(10);

    // This time, read the high byte contents of the data bus
    data_pins_as_inputs();
    delay(100);
    unsigned int data_low = read_data_bus();
    Serial.println("Data low: " + String(data_low));

    // Again, we're going to replace the contents of the data bus with instructions for the chip to jump to the required address
    data_pins_as_outputs();
    delay(10);
    write_data_bus(0xF9); // Mode/register setting for direct addressing, long word (four byte) address
    delay(10);
    dtack_pulse();
    delay(10);

    // Now, we're giving the long word value for the address to jump to
    unsigned int address_4th_byte = address >> 24;
    write_data_bus(address_4th_byte);
    dtack_pulse();

    unsigned int address_3rd_byte = address >> 16;
    write_data_bus(address_3rd_byte);
    dtack_pulse();
    delay(10);

    unsigned int address_2nd_byte = address >> 8;
    write_data_bus(address_2nd_byte);
    dtack_pulse();
    delay(10);

    unsigned int address_1st_byte = (int)address;
    write_data_bus(address_1st_byte);
    dtack_pulse();
    delay(10);

    // The m68k will now execute the JMP instruction and read the contents of the ROM at the next specified address
    delay(5);
    unsigned long data_long = ((unsigned long)data_high << 8) + data_low;

    unsigned long read_address = read_address_bus(); 
    Serial.println("Address " + String(address) + " at read address " + String(read_address)  + " has data " + String(data_long));
    hash_value += data_long;
    delay(100);
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

  // First of all, set the right pin modes
  data_pins_as_outputs();
  // Reset the m68k system
  reset_setup();

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

  if (hash_first_pass == hash_second_pass) {
    Serial.println("ROM test success! Hash values for both ROM passes were identical.");
  } else {
    Serial.println("Error! ROM hash of pass 1: " + String(hash_first_pass) + " was not identical to pass 2: " + String(hash_second_pass));
  }

  reset_setup();
}