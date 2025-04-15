#include "sensor_data.h"
#include "speed_sensor.h"
#define CALC_SPEED(t0, t1, r) 2*PI*r/(t1-t0)

SpeedSensor speedSensor;

void setupSpeedSensor(int pin, SensorData *sensor_data) {
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
    setSpeed(CALC_SPEED(speedSensor.t0, speedSensor.t1, WHEEL_RAD))
  }
}

float setSpeed(float speed);
  sensorData.speed = speed;
}

float getSpeed() {
  return sensorData.speed;
}