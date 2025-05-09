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
void setTargetMotorRPMPercent(float speedPercent) {
  if (speedPercent <= 0.001) {
    motor.writeMicroseconds(1500);
    return;
  }
  int correctedValue = (int) map((int) (100*speedPercent), 0, 100*100, 1600, 2000);
  correctedValue = constrain(correctedValue, 1500, 2000);
  motor.writeMicroseconds(correctedValue);
}