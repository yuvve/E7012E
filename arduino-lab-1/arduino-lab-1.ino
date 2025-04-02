#define F_TO_P_MILLI(f) 1000.0/f

volatile int counter = 0;
volatile float t = 0.0;
volatile float t0 = 0.0;
float f = 1.0;
int brightness = 1;
bool check = false;

void setup() {
  pinMode(13, OUTPUT); // Set pin 13 as output
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), countingISR, RISING);
  Serial.begin(9600);
}

void loop() {
  analogWrite(13, brightness); // Turn the LED on
  delay(F_TO_P_MILLI(f)/2);            // Wait one second
  analogWrite(13, 0);  // Turn the LED off
  delay(F_TO_P_MILLI(f)/2);            // Wait one second 
  // if (Serial.available()) {
    // f = Serial.readStringUntil('\n').toFloat();
  //}
  if (Serial.available()) {
    brightness = Serial.readStringUntil('\n').toFloat();
  }
  if (check) {
    Serial.print("Counter: ");
    Serial.println(counter);
    Serial.print("Periodtid: ");
    Serial.println((t-t0)/1000000.0);
    Serial.print("Frekvens: ");
    Serial.println(1.0/((t-t0)/1000000.0));
    check = false;
  }
}

void countingISR() {
  counter ++;
  t0 = t;
  t = micros();
  check = true;
}