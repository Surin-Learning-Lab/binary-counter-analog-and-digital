#include <Servo.h>
#include <TM1637Display.h>

// Servo setup
Servo servos[8];
int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 13}; // Update with actual pins connected to servos

// Display setup
#define CLK 9 // Clock pin for TM1637
#define DIO 10 // Data pin for TM1637
TM1637Display display(CLK, DIO);

int buttonPin = 1;
int lastButtonState = LOW;      // the previous reading from the input pin
int currentButtonState = LOW;   // the current reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

bool runCount = false; // State variable to control counting

void setup() {
  // Initialize serial
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // Initialize display
  display.setBrightness(0x0f); // Set brightness level (0x00 to 0x0f)
  
  Serial.println("Setup complete");
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed:
    if (reading != currentButtonState) {
      currentButtonState = reading;

      // only toggle the runCount if the new button state is HIGH
      if (currentButtonState == HIGH) {
        runCount = !runCount; // Toggle running state
        if (runCount) {
          attachServos(); // Attach servos when we start counting
        }
      } else {
        runCount = false; // Stop counting and detach servos, clear display
        detachServos();
        display.clear();
      }
    }
  }

  lastButtonState = reading;

  // Start counting when runCount is true
  if (runCount) {
    for (int number = 0; number <= 255; number++) {
      updateDisplayAndServos(number);
      delay(1500); // Adjust time as needed
    }
    runCount = false; // Reset the runCount after one complete cycle
    detachServos();   // Detach servos and clear display once done
    display.clear();
  }
}

void attachServos() {
  for (int i = 0; i < 8; i++) {
    servos[i].attach(servoPins[i]);
  }
}

void detachServos() {
  for (int i = 0; i < 8; i++) {
    servos[i].detach();
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
