// This program was adapted from Ben Eater's wonderful 6502 project at https://eater.net/6502
// It supports the 20-bit address bus of the 68008 however, instead of the 16 lines of the 6502.
// Check out his video on using the data logger on https://youtu.be/LnzuMJLZRdU?t=513

const char ADDRESS_PINS[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 23, 25, 27, 29};
const char DATA_PINS[] = {39, 41, 43, 45, 47, 49, 51, 53};
#define CLOCK 2
#define READ_WRITE 3

void setup() {
  for (int n = 0; n < strlen(ADDRESS_PINS); n += 1) {
    pinMode(ADDRESS_PINS[n], INPUT);
  }
  for (int n = 0; n < strlen(DATA_PINS); n += 1) {
    pinMode(DATA_PINS[n], INPUT);
  }
  pinMode(CLOCK, INPUT);
  pinMode(READ_WRITE, INPUT);

  attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
  
  Serial.begin(57600);
}

void onClock() {
  char output[15];

  unsigned int address = 0;
  for (int n = 0; n < strlen(ADDRESS_PINS); n += 1) {
    int bit = digitalRead(ADDRESS_PINS[n]) ? 1 : 0;
    Serial.print(bit);
    address = (address << 1) + bit;
  }
  
  Serial.print("   ");
  
  unsigned int data = 0;
  for (int n = 0; n < strlen(DATA_PINS); n += 1) {
    int bit = digitalRead(DATA_PINS[n]) ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  sprintf(output, "   %05x  %c %02x", address, digitalRead(READ_WRITE) ? 'r' : 'W', data);
  Serial.println(output);  
}

void loop() {
}
