// This sketch can be used to send a decimal number to the analog display and display that number in binary

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <SevSeg.h>

// WiFi settings
const char* ssid = "************";
const char* password = "*********";

// Create web server on port 80
ESP8266WebServer server(80);

// Servo setup
Servo servos[8];
int servoPins[] = {D1, D2, D3, D4, D5, D6, D7, D8}; // Update with actual GPIO numbers

// SevSeg setup
SevSeg sevseg;
byte numDigits = 4;
byte digitPins[] = {D6, D7, D8, D9}; // Update with actual GPIO numbers
byte segmentPins[] = {D4, D5, D6, D7, D8, D3, D2, D1}; // Update with actual GPIO numbers
bool resistorsOnSegments = false; 
byte hardwareConfig = COMMON_CATHODE;

void setup() {
  // Initialize serial
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize servos
  for (int i = 0; i < 8; i++) {
    servos[i].attach(servoPins[i]);
  }

  // Initialize 7-segment display
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  // Setup server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/number", HTTP_POST, handleNumberInput);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Servo Control</title></head><body>"
                "<h1>Enter a Number</h1>"
                "<form action='/number' method='post'>"
                "<input type='number' name='number' min='0' max='255'>"
                "<input type='submit' value='Submit'>"
                "</form>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleNumberInput() {
  if (server.hasArg("number")) {
    int number = server.arg("number").toInt();
    updateDisplayAndServos(number);
    server.sendHeader("Location", String("/"), true);
    server.send(302, "text/plain", "Updated successfully, going back...");
  } else {
    server.send(400, "text/plain", "Bad Request: Missing 'number' argument");
  }
}

void updateDisplayAndServos(int number) {
  // Update 7-segment display
  sevseg.setNumber(number, 0);
  sevseg.refreshDisplay();

  // Update servos based on binary representation
  for (int i = 0; i < 8; i++) {
    int position = (number & (1 << i)) ? 135 : 45;
    servos[i].write(position);
  }
}

