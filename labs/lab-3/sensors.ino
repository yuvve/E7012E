#include "sensors.h"
#define CALC_SPEED(t0, t1, r) 2*PI*r/(t1-t0)

SensorData sensorData;
SpeedSensor speedSensor;

void setupSensorData() {
  sensorData = {0.0f};
}

void setupSpeedSensor(unsigned int pin) {
  pinMode(pin, INPUT);
  speedSensor = {0, (int)(micros()/1000000), 0};
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  if DEBUG {
    Serial.println("Speed sensor initialized");
  }
}

void speedSensorISR() {
  speedSensor.pulses++;
  if (speedSensor.pulses % 4) {
    speedSensor.t0 = speedSensor.t1;
    speedSensor.t1 = (int) (micros()/1000000);
    setSpeed(CALC_SPEED(speedSensor.t0, speedSensor.t1, (WHEEL_DIAM_CM/2)*100));
  }
}

void setSpeed(float speed){
  noInterrupts();
  sensorData.speed = speed;
  interrupts();
}

float getSpeed() {
  noInterrupts();
  float speed = sensorData.speed;
  interrupts();
  return speed;
}
