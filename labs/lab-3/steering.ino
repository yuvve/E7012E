#include <Servo.h>

void setupSteering(Servo *servo, int pin) {
    servo->attach(pin);
    servo->write(90);
  if DEBUG {
    Serial.println("Steering initialized");
  }
}

void changeAngle(Servo *servo, int angle) {
  int corrected_value = map(angle, -90, 0, 90, 180);
  servo->writeMicroseconds(corrected_value);
  if DEBUG {
    Serial.print("Changing angle to: ");
    Serial.println(corrected_value);
  }
}