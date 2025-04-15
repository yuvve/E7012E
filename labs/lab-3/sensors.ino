#include "sensors.h"
#define DEBUG (1)
#define CALC_SPEED(t0, t1, r) 2*PI*r/(t1-t0)

SensorData sensorData;
SpeedSensor speedSensor;

void setupSpeedSensor(int pin) {
  pinMode(pin, INPUT);
  speedSensor = {0, micros(), 0};
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  if DEBUG {
    Serial.println("Speed sensor initialized");
  }
}

void speedSensorISR() {
  speedSensor.pulsesSinceLastUpdate++;
  if (speedSensor.pulsesSinceLastUpdate % 4) {
    speedSensor.t0 = speedSensor.t1;
    speedSensor.t1 = micros();
    speedSensor.pulsesSinceLastUpdate = 0;
    setSpeed(CALC_SPEED(speedSensor.t0, speedSensor.t1, WHEEL_RAD));
  }
}

void setupSensorData() {
  sensorData.speed = 0;
}

void setSpeed(float speed){
  sensorData.speed = speed;
}

float getSpeed() {
  return sensorData.speed;
}
