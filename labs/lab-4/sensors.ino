#include "sensors.h"

SensorData sensorData;
SpeedSensor speedSensor;
TriggerData rightTriggerData, leftTriggerData;
volatile EchoData rightEchoData, leftEchoData;
ProximitySensor rightProximitySensor, leftProximitySensor;
enum TriggerState { TRIGGER_LEFT, TRIGGER_RIGHT };
TriggerState triggerState;

void setupSensorData() {
  sensorData = {0.0f};
}

void setupSpeedSensor(unsigned int pin) {
  pinMode(pin, INPUT);
  speedSensor = {micros(), 0, 0};
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  DEBUG_PRINTLN("Speed sensor initialized");
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

void setupRightProximitySensor() {
  pinMode(RIGHT_PROXIMITY_TRIGGER_PIN, OUTPUT);
  pinMode(RIGHT_PROXIMITY_ECHO_PIN, INPUT);

  rightProximitySensor.triggerData = {RIGHT_PROXIMITY_TRIGGER_PIN, false, 0};
  rightProximitySensor.echoData = {RIGHT_PROXIMITY_ECHO_PIN, 0, 0, 0.0f};
  attachInterrupt(digitalPinToInterrupt(RIGHT_PROXIMITY_ECHO_PIN), rightProximityISR, CHANGE);
  DEBUG_PRINTLN("Right proximity sensor initialized");
}

void setupLeftProximitySensor() {
  pinMode(LEFT_PROXIMITY_TRIGGER_PIN, OUTPUT);
  pinMode(LEFT_PROXIMITY_ECHO_PIN, INPUT);

  leftProximitySensor.triggerData = {LEFT_PROXIMITY_TRIGGER_PIN, false, 0};
  leftProximitySensor.echoData = {LEFT_PROXIMITY_ECHO_PIN, 0, 0, 0.0f};
  attachInterrupt(digitalPinToInterrupt(LEFT_PROXIMITY_ECHO_PIN), leftProximityISR, CHANGE);
  DEBUG_PRINTLN("Left proximity sensor initialized");
}

void setupProximitySensors() {
  setupRightProximitySensor();
  setupLeftProximitySensor();
  triggerState = TRIGGER_RIGHT; // Start with right sensor
}

// Arduino API does not support detecting the interrupt pin, so we need to use
// sepparate ISR for each pin
void leftProximityISR() {
  handleProximityISR(leftProximitySensor);
}

// Arduino API does not support detecting the interrupt pin, so we need to use
// sepparate ISR for each pin
void rightProximityISR() {
  handleProximityISR(rightProximitySensor);
}

void handleProximityISR(ProximitySensor sensor) {
  if (digitalRead(sensor.echoData.echoPin)) {
    sensor.echoData.tEchoStart = micros();    // Rising edge
  } else {
    sensor.echoData.tEchoEnd = micros();    // Falling edge
    uint width = sensor.echoData.tEchoEnd - sensor.echoData.tEchoStart;
    float range = width * (SPEED_OF_SOUND_CM_US / 2);
    sensor.echoData.range = range;
  }
}

bool triggerProximitySensor() {
  uint32_t now = micros();
  ProximitySensor &sensor = (triggerState == TRIGGER_LEFT) ? leftProximitySensor : rightProximitySensor;

  if (!sensor.triggerData.isTriggered) {
    digitalWrite(sensor.triggerData.triggerPin, HIGH);
    sensor.triggerData.tTriggered = now;
    sensor.triggerData.isTriggered = true;
  } 
  else if (now - sensor.triggerData.tTriggered >= PROXIMITY_PULSE_DURATION_US) {
    digitalWrite(sensor.triggerData.triggerPin, LOW);
    sensor.triggerData.isTriggered = false;

    // Alternate to the other sensor for next trigger
    triggerState = (triggerState == TRIGGER_LEFT) ? TRIGGER_RIGHT : TRIGGER_LEFT;
    return true;
  }

  return false; 
}