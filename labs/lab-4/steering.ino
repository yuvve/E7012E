#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(int angle) {
  int correctedValue = map(angle, -90, 90, 40, 88);
  correctedValue = constrain(correctedValue, 40, 88);
  steering.write(correctedValue);
}

void freeSteering(int actuation) {
  steering.write(actuation);
}