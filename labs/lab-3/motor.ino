#include <Servo.h>

Servo motor;

void setupMotor(int pin) {
    motor.attach(pin,1000,2000);
    motor.writeMicroseconds(1500);
  if DEBUG {
      Serial.println("Motor initialized");
  }
}

void setTargetMotorRPMPercent(int speedPercent) {
  int corrected_value = map(speedPercent, -100, 100, 1000, 2000);
  corrected_value = constrain(corrected_value, 1000, 2000);
  motor.writeMicroseconds(corrected_value);
  if DEBUG {
    Serial.print("Setting target RPM to: ");
    Serial.print(corrected_value);
    Serial.println("% of max");
  }
}