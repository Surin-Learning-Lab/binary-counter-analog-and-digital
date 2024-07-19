#include <Servo.h>
#include <TM1637Display.h>

// Servo setup
Servo servos[8];
int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 13}; // Update with actual pins connected to servos

// Display setup
#define CLK 9 // Clock pin for TM1637
#define DIO 10 // Data pin for TM1637
TM1637Display display(CLK, DIO);

void setup() {
  // Initialize serial
  Serial.begin(9600);

  // Initialize servos
  for (int i = 0; i < 8; i++) {
    servos[i].attach(servoPins[i]);
  }

  // Initialize display
  display.setBrightness(0x00); // Set brightness level (0x00 to 0x0f)
  
  Serial.println("Setup complete");
}

void loop() {
  for (int number = 0; number <= 255; number++) {
    updateDisplayAndServos(number);
    delay(5000); // 5 second delay
  }
}

void updateDisplayAndServos(int number) {
  // Update servos based on binary representation
  for (int i = 0; i < 8; i++) {
    int position = (number & (1 << i)) ? 140 : 40;
    servos[i].write(position);
  }
  
  // Update 7-segment display with decimal number
  display.showNumberDec(number, true); // Display the number in decimal format
  
  Serial.print("Number: ");
  Serial.println(number);
}

