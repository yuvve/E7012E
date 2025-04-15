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
  int corrected_value = map(angle, -90, 0, 90, 180);
  steering.writeMicroseconds(corrected_value);
  if DEBUG {
    Serial.print("Changing angle to: ");
    Serial.println(corrected_value);
  }
}