#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(int angle) {
  int correctedValue = map(angle, -90, 90, 0, 50);
  correctedValue = constrain(correctedValue, 0, 50);
  steering.write(correctedValue);
}

void freeSteering(int actuation) {
  steering.write(actuation);
}
