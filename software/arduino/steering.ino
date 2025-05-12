#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(float angle) {
  float correctedValue = map(angle, STEERING_MIN_ANGLE, STEERING_MAX_ANGLE, STEERING_MIN_SERVO, STEERING_MAX_SERVO);
  correctedValue = constrain(correctedValue, STEERING_MIN_SERVO, STEERING_MAX_SERVO);
  steering.write((int) correctedValue);
}

void freeSteering(int actuation) {
  steering.write(actuation);
}
