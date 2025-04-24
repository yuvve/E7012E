#include "sensors.h"
#define WHEEL_CONSTANT PI*WHEEL_DIAM_CM

SensorData sensorData;
SpeedSensor speedSensor;

void setupSensorData() {
  sensorData = {0.0f};
}

void setupSpeedSensor(unsigned int pin) {
  pinMode(pin, INPUT);
  speedSensor = {0, micros(), 0};
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  if DEBUG {
    Serial.println("Speed sensor initialized");
  }
}

void speedSensorISR() {
  speedSensor.pulses++;
  if (speedSensor.pulses % 4) {
    speedSensor.t0 = speedSensor.t1;
    speedSensor.t1 = micros();
    float speed = WHEEL_CONSTANT/((float) speedSensor.t1-speedSensor.t0);
    setSpeed(speed);
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
