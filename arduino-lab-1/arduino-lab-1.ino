void setup() {
  pinMode(13, OUTPUT); // Set pin 13 as output
}

void loop() {
  digitalWrite(13, HIGH); // Turn the LED on
  delay(500);            // Wait one second
  digitalWrite(13, LOW);  // Turn the LED off
  delay(500);            // Wait one second
  
}