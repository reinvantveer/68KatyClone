#include <Arduino.h>


unsigned long hash_rom_space() {
  /*
  The ROM test goes through the address space of the ROM chip with 4096 bit jumps.
  Each time it jumps to the next location, it reads the byte stored there and increases a hash value with the data byte.
  */

  unsigned long rom_address_space_start = 0x0000;
  // unsigned long rom_address_space_end = 0x77FFF;
  unsigned long rom_address_space_end = 0xFFFF;

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

    // Set up the data bus as output for the instructions we're about to feed
    data_pins_as_outputs();

    // We're going to execute a MOVE.W effective_address, data_register to request the two bytes at the requested address
    // The bits for this operation: 0 0         1 1   0 0 0                 0 0 0       1 1 1       0 0 0            
    // Format                       instruction size  destination register  dest. mode  source mode source register           
    // Semantics                    move        word  dest:data_register    D0          word        contents at address  
    // Hex code                     0x30                                        0x38   


    // JMP command hex opcodes: 0x4E 0xF8
    write_data_bus(0x30); // Basically the MOVE opcode and the destination register
    // write_data_bus(0x4E); 
    dtack_pulse();

    write_data_bus(0x38); // The rest of the destination and source addressing and modes
    // write_data_bus(0xF8); // The rest of the destination and source addressing and modes
    dtack_pulse();

    // We're going to place the contents of the data bus with instructions for the chip to read from the required address
    unsigned char address_2nd_byte = (char)(address >> 8);
    Serial.println("High byte 0x" + String(address_2nd_byte, HEX));
    write_data_bus(address_2nd_byte);
    dtack_pulse();

    unsigned char address_1st_byte = (char)address;
    Serial.println("Low byte 0x" + String(address_1st_byte, HEX));
    write_data_bus(address_1st_byte);
    dtack_pulse();
    
    // Now, we're allowing the ROM to output the data from the requested adress, which we're going to read.
    data_pins_as_inputs();
    unsigned int data_now = read_data_bus();
    Serial.println("Address 0x" + String(read_address_bus(), HEX) + " has data 0x" + String(data_now, HEX));
    hash_value += data_now;
    dtack_pulse();

    // Allow the second byte of the operation to pass
    data_now = read_data_bus();
    Serial.println("Address 0x" + String(read_address_bus(), HEX) + " has data 0x" + String(data_now, HEX));
    hash_value += data_now;
    dtack_pulse();
    Serial.println();
    delay(100);
  }

  Serial.println();
  return hash_value; 
}

void rom_test() {
  /*
  The ROM test gathers a hash value for part of the contents of the ROM. 
  The test then repeats this process, to a second hash.
  Once the entire ROM address space is visited twice, the hash values should not be 0 and should be equal.
  */

  data_pins_as_outputs();
  reset_setup();

  Serial.println("Hashing first iteration");
  unsigned long hash_first_pass = hash_rom_space();
  if (! hash_first_pass) {
    Serial.println("Error! ROM hash for the first pass was 0.");
    return;
  }

  reset_setup();

  Serial.println("Hashing second iteration");
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

  // reset afterwards
  // arduino_reset();
  reset_setup();
}