#include <Servo.h>

void setupSteering(Servo *self, int pin) {
    self->attach(pin);
    self->write(90);
  if DEBUG {
    Serial.println("Steering initialized");
  }
}

void changeAngle(Servo *self, int angle) {
  int corrected_value = map(angle, -90, 0, 90, 180);
  self->writeMicroseconds(corrected_value);
  if DEBUG {
    Serial.print("Changing angle to: ");
    Serial.println(corrected_value);
  }
}