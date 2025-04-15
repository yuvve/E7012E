#include <Servo.h>

void setupMotor(Servo *servo, int pin) {
    servo->attach(pin,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    servo->writeMicroseconds(1500);
  if DEBUG {
      Serial.println("Motor initialized");
  }
}

void setTargetMotorRPMPercent(Servo *servo, int speedPercent) {
  int corrected_value = map(speedPercent, -100, 1000, 100, 2000);
  servo->writeMicroseconds(corrected_value);
  if DEBUG {
    Serial.print("Setting target RPM to: ");
    Serial.print(corrected_value);
    Serial.println("% of max");
  }
}