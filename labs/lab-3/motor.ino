#include <Servo.h>

void setupMotor(Servo *self, int pin) {
    self->attach(pin,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    self->writeMicroseconds(1500);
  if DEBUG {
      Serial.println("Motor initialized");
  }
}

void changeSpeed(Servo *self, int speedProcent) {
  int corrected_value = map(speedProcent, -100, 1000, 100, 2000);
  self->writeMicroseconds(corrected_value);
  if DEBUG {
    Serial.print("Changing speed to: ");
    Serial.println(corrected_value);
  }
}