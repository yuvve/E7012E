#include <Servo.h>

Servo steering;

void setupSteering(int pin) {
    steering.attach(pin);
    steering.write(90);
  if DEBUG {
    Serial.println("Steering initialized");
  }
}

void changeSteeringAngle(int angle) {
  int correctedValue = map(angle, -90, 90, 0, 180);
  steering.write(correctedValue);
  if DEBUG {
    Serial.print("Changing angle to: ");
    Serial.println(correctedValue);
  }
}