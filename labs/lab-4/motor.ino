Servo motor;

void setupMotor(int pin) {
  motor.attach(pin,1000,2000);
  motor.writeMicroseconds(1500);
  DEBUG_PRINTLN("Motor initialized");
}

/**
* Sends requested RPM to the ESC
*
* 0 maps to 0
* Anything above zero maps to a mapping function that starts at around 24%, 
* as anything under that is not enough to make the motor move.
**/
void setTargetMotorRPMPercent(int speedPercent) {
  if (speedPercent == 0) {
    motor.writeMicroseconds(1500);
    return;
  }
  int correctedValue = map(speedPercent, 0, 100, 1600, 2000);
  correctedValue = constrain(correctedValue, 1500, 2000);
  motor.writeMicroseconds(correctedValue);
}

float speedLimiter(float maxSpeed, float frontDistanceToWallCm) {
  if (frontDistanceToWallCm <= HARD_MIN_DISTANCE_TO_FRONT_WALL_CM) {
    return 0.0;
  }
  if (frontDistanceToWallCm > SOFT_MIN_DISTANCE_TO_FRONT_WALL_CM) {
    return maxSpeed;
  }
  float factor = map(frontDistanceToWallCm, 0, SOFT_MIN_DISTANCE_TO_FRONT_WALL_CM, 0, 1);
  return factor*maxSpeed;
}