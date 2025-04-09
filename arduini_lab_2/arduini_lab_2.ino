#include <Servo.h>

Servo myservo;

void setup() {
  // put your setup code here, to run once:
  pinMode(52, INPUT);
  pinMode(13, OUTPUT);
  myservo.attach(12);
  myservo.write(90);
  attachInterrupt(digitalPinToInterrupt(52), detectMagnet, RISING); 
  Serial.begin(9600);
}

volatile int magCounter = 0;
volatile float t = 1.0;
volatile float t0 = 0.0;
bool check = false;
volatile int pwm = 255;
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
      pwm = value;
      analogWrite(13,pwm);
      Serial.println(pwm);
    }
    else if (cmd == 'S') {
      angle = value;
      myservo.write(angle);
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