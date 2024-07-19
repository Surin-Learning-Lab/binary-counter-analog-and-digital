#include <Servo.h>

// Servo setup
Servo servos[8];
int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 13}; // Update with actual pins connected to servos

void setup() {
  // Initialize serial
  Serial.begin(9600);

  // Initialize servos
  for (int i = 0; i < 8; i++) {
    servos[i].attach(servoPins[i]);
  }

  Serial.println("Setup complete");
}

void loop() {
  for (int number = 0; number <= 255; number++) {
    updateDisplayAndServos(number);
    delay(5000); // 1 second delay
  }
}

void updateDisplayAndServos(int number) {
  // Update servos based on binary representation
  for (int i = 0; i < 8; i++) {
    int position = (number & (1 << i)) ? 140 : 40;
    servos[i].write(position);
  }
  Serial.print("Number: ");
  Serial.println(number);
}
