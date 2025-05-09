#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(float angle) {
  float correctedValue = map(angle, -90.0, 90.0, 0.0, 50.0);
  correctedValue = constrain(correctedValue, 0.0, 50.0);
  steering.write((int) correctedValue);
}

void freeSteering(int actuation) {
  steering.write(actuation);
}
