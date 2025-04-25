#include "sensors.h"
#define NR_MAGNETS 4.0f
#define WHEEL_CONSTANT PI*WHEEL_DIAM_CM/100.0 // 2*PI*r = 2*PI*diam/2 = PI*diam
#define METERS_PER_PULSE WHEEL_CONSTANT/NR_MAGNETS

SensorData sensorData;
SpeedSensor speedSensor;

void setupSensorData() {
  sensorData = {0.0f};
}

void setupSpeedSensor(unsigned int pin) {
  pinMode(pin, INPUT);
  speedSensor = {micros(), 0, 0};
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  if DEBUG {
    Serial.println("Speed sensor initialized");
  }
}

void speedSensorISR() {
  speedSensor.pulses1++;
}

// Calculates the new current speed and update the internal data structure
void calcCurrSpeed(){
  noInterrupts();
  uint32_t t1 = micros();
  double deltaPulses = (double) (speedSensor.pulses1 - speedSensor.pulses0);
  double deltaTime = ((double) (t1-speedSensor.t0)/1000000.0);
  double speed = (deltaPulses/deltaTime)*METERS_PER_PULSE;
  speedSensor.t0 = t1;
  speedSensor.pulses0 = speedSensor.pulses1;
  sensorData.speed = speed;
  interrupts();
}

float getSpeed() {
  noInterrupts();
  float speed = sensorData.speed;
  interrupts();
  return speed;
}
