#include <Servo.h>

Servo steering;

void setupSteering(unsigned int pin) {
  steering.attach(pin);
  steering.write(90);
  DEBUG_PRINTLN("Steering initialized");
}

void changeSteeringAngle(int angle) {
  int correctedValue = constrain(angle, 0, 110);
  steering.write(correctedValue);
  DEBUG_PRINTF("Changing angle to: %d", correctedValue);
}