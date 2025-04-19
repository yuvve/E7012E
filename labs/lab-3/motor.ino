Servo motor;

void setupMotor(int pin) {
    motor.attach(pin,1000,2000);
    motor.writeMicroseconds(1500);
  if DEBUG {
      Serial.println("Motor initialized");
  }
}

void setTargetMotorRPMPercent(int speedPercent) {
  int correctedValue = map(speedPercent, -100, 100, 1000, 2000);
  correctedValue = constrain(correctedValue, 1000, 2000);
  motor.writeMicroseconds(correctedValue);
}