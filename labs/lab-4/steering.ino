#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(int angle) {
  int correctedValue = constrain(angle, -STEERING_MAX_ANGLE_DEG, STEERING_MAX_ANGLE_DEG);
  correctedValue = map(correctedValue, -90, 90, 0, 50);
  correctedValue = constrain(correctedValue, 0, 50);
  steering.write(correctedValue);
}

void freeSteering(int actuation) {
  steering.write(actuation);
}
