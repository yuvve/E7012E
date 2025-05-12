#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(float angle) {
  float correctedValue = map(angle, STEERING_MAP_MIN_FROM_SERVO, STEERING_MAP_MAX_FROM_SERVO, STEERING_MAP_MIN_TO_SERVO, STEERING_MAP_MAX_TO_SERVO);
  correctedValue = constrain(correctedValue, STEERING_MAP_MIN_TO_SERVO, STEERING_MAP_MAX_TO_SERVO);
  steering.write((int) correctedValue);
}

void freeSteering(int actuation) {
  steering.write(actuation);
}
