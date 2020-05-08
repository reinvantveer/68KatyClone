#include <Arduino.h>

unsigned long hash_rom_space() {
  return 0; // Dummy value for now
}

void rom_test() {
  /*
  The ROM test goes through the address space of the ROM chip with 4096 bit jumps.
  Each time it jumps to the next location, it reads the byte stored there and multiplies a hash value.
  The test then repeats this process, to a second hash.
  Once the entire ROM address space is visited twice, the hash values should not be 0 and should be equal.
  */

  unsigned long hash_first_pass = hash_rom_space();
  if (! hash_first_pass) {
    Serial.println("Error! ROM hash for the first pass was 0.");
    return;
  }

  unsigned long hash_second_pass = hash_rom_space();
  if (! hash_second_pass) {
    Serial.println("Error! ROM hash for the second pass was 0.");
    return;
  }

  if (!(hash_first_pass == hash_second_pass)) {
    Serial.println("Error! ROM hashes of both passes was not identical.");
    return;
  }

}