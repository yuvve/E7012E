Servo motor;

void setupMotor(int pin) {
    motor.attach(pin,1000,2000);
    motor.writeMicroseconds(1500);
  if DEBUG {
      Serial.println("Motor initialized");
  }
}

void setTargetMotorRPMPercent(int speedPercent) {
  if (speedPercent == 0) {
    motor.writeMicroseconds(1500);
    return;
  }
  int correctedValue = map(speedPercent, 0, 100, 1600, 2000);
  correctedValue = constrain(correctedValue, 1500, 2000);
  motor.writeMicroseconds(correctedValue);
}