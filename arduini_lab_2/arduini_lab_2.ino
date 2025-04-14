#include <Servo.h>

Servo steering;
Servo ESC;


void setup() {
  // put your setup code here, to run once:
  pinMode(52, INPUT);
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC.writeMicroseconds(1500);
  Serial.begin(9600);
  steering.attach(10);
  steering.write(90);
  attachInterrupt(digitalPinToInterrupt(52), detectMagnet, RISING); 
  Serial.begin(9600);
}

volatile int magCounter = 0;
volatile float t = 1.0;
volatile float t0 = 0.0;
bool check = false;
volatile int escValue;
volatile int angle = 90;

void loop() {
  // put your main code here, to run repeatedly:
  float d = 0.05;
  float v = 3.1415*d/(t-t0)*1000000;
  if (check) {
    Serial.print("magCounter: ");
    Serial.print(magCounter);
    Serial.print(", tid:");
    Serial.print((t-t0)/1000000.0);
    Serial.print(", hastighet: ");
    Serial.println(v);
    check = false;
  }           

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    char cmd = input.charAt(0);
    int value = input.substring(1).toInt();

    if (cmd == 'P') {
      escValue = value;
      ESC.write(escValue);
      ESC.writeMicroseconds(escValue); 
    }
    else if (cmd == 'S') {
      angle = value;
      steering.write(angle);
      Serial.println(angle);
    }
    else {
      Serial.println("skriv S för servo och P för pwm");
    }
  }
}

void detectMagnet() {
  magCounter ++;
  t0 = t;
  t = micros();
  check = true;
}