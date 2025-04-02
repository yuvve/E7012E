#define F_TO_P_MILLI(f) 1000.0/f

int counter = 0;
float t = 0.0;
float t0 = 0.0;
float f = 1.0;

void setup() {
  pinMode(13, OUTPUT); // Set pin 13 as output
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), countingISR, RISING);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13, HIGH); // Turn the LED on
  delay(F_TO_P_MILLI(f)/2);            // Wait one second
  digitalWrite(13, LOW);  // Turn the LED off
  delay(F_TO_P_MILLI(f)/2);            // Wait one second 
  if (Serial.available()) {
    f = Serial.readStringUntil('\n').toFloat();
  }
  Serial.print("Counter: ");
  Serial.println(counter);
  Serial.print("Periodtid: ");
  Serial.println((t-t0)/1000000.0);
  Serial.print("Frekvens: ");
  Serial.println(f);
}

void countingISR() {
  counter ++;
  t0 = t;
  t = micros();
}