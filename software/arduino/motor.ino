Servo motor;

void setupMotor(int pin) {
  motor.attach(pin,1000,2000);
  motor.writeMicroseconds(1500);
  DEBUG_PRINTLN("Motor initialized");
}

void setTargetMotorRPMPercent(float speedPercent) {
  int correctedValue = (int) map((int) (100*speedPercent), -100*100, 100*100, 1000, 2000);
  correctedValue = constrain(correctedValue, 1000, 2000);
  motor.writeMicroseconds(correctedValue);
}