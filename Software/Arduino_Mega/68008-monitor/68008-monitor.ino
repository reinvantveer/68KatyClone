// Arduino Mega

// This program was adapted from Ben Eater's wonderful 6502 project at https://eater.net/6502
// In its adapted form, it supports the 20-bit address bus of the 68008 however, instead of the 16 lines of the 6502.
// Check out his video on using the data logger on https://youtu.be/LnzuMJLZRdU?t=513

// Pin mapping for address lines
const String ADDRESS_LINES[] = {
    "A0",  "A1",  "A2",  "A3",  "A4",  "A5",  "A6",  "A7",  "A8",  "A9",
    "A10", "A11", "A12", "A13", "A14", "A15", "A16", "A17", "A18", "A19"
};
const char ADDRESS_PINS[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 53, 51, 49, 47};


//Mapping for data pins
const String DATA_LINES[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"};
const char DATA_PINS[] =  {31,   33,   35,   37,   39,   41,  43,  45};

//#define CLOCK 29
#define READ_WRITE 27
// DTACK needs to be on a special-purpose interruptable pin: 
// See https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
#define DTACK 21

void setup() {
  Serial.begin(2000000);

  // Initialize the address pins as input
  Serial.println();
  Serial.print("Watching address bus of size: ");
  Serial.println(sizeof(ADDRESS_PINS));

  for (int n = 0; n < sizeof(ADDRESS_PINS); n++) {
    pinMode(ADDRESS_PINS[n], INPUT);
    Serial.println("Address line " + ADDRESS_LINES[n] + " set as input to Arduino pin " + String(int(ADDRESS_PINS[n])) );
  }

  // Initialize the data pins as inputs
  Serial.println();
  Serial.print("Watching data bus of size: ");
  Serial.println(sizeof(DATA_PINS));

  for (int n = 0; n < sizeof(DATA_PINS) ; n++) {
    pinMode(DATA_PINS[n], INPUT);
    Serial.println("Data line " + DATA_LINES[n] + " set as input to Arduino pin " + String(int(DATA_PINS[n])) );
  }

  // The Data acknowledge line is an input too - it will trigger an interrupt (see below)
  Serial.println();
  pinMode(DTACK, INPUT);
  Serial.println("DTACK pin " + String(DTACK) + " set as input");
  // The read/write pin off the 68k as input as well
  Serial.println("READ/WRITE pin " + String(READ_WRITE) + " set as input");
  pinMode(READ_WRITE, INPUT);

  // We'll trigger a "readBuses" function on each falling edge of the data acknowledge pin
  // We're taking the falling edge since DTACK is an active-low pin.
  attachInterrupt(digitalPinToInterrupt(DTACK), readBuses, RISING);
  
  Serial.println();
  Serial.println("Address (binary)     | Data (binary) | Addres (hex) | Read/write | Data (hex)");
}

void readBuses() {
  unsigned int address = 0;
  for (int n = sizeof(ADDRESS_PINS) - 1; n >= 0; n--) {
    int bit = digitalRead(ADDRESS_PINS[n]) ? 1 : 0;
    Serial.print(bit);
    address = (address << 1) + bit;
  }
  
  Serial.print("   ");
  
  unsigned int data = 0;
  for (int n = sizeof(DATA_PINS) - 1; n >= 0; n --) {
    int bit = digitalRead(DATA_PINS[n]) ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  // Print out the address in hexadecimal
  Serial.print("        ");
  char pretty_print[100];
  sprintf(pretty_print, "%05X", address);
  Serial.print(pretty_print);
  Serial.print("          ");

  // Print out the status of the read/write pin
  Serial.print(digitalRead(READ_WRITE) ? 'r' : 'W');
  Serial.print("            ");

  // Print out the data bus contents as hexadecimal
  sprintf(pretty_print, "%02X", data);
  Serial.println(pretty_print);
}

void loop() {
}
