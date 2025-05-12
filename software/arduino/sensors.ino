#include "sensors.h"

SensorData sensorData;
SpeedSensor speedSensor;

void setupSensorData() {
  sensorData.speed = 0.0f;
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

void setupProximitySensor(uint echoPin, uint triggerPin, ProximitySensor& sensor, void (*isr)()) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(echoPin), isr, CHANGE);

  sensor.triggerData = {triggerPin, false, 0};
  // Volatile fields cant be brace-initialized, so we need to do it field by field
  sensor.echoData.echoPin = echoPin;
  sensor.echoData.tEchoStart = 0;
  sensor.echoData.tEchoEnd = 0;
  sensor.echoData.range = 0.0f;

  DEBUG_PRINTF("Proximity sensor initialized with trigger pin: %d, echo pin: %d", triggerPin, echoPin);
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

// Arduino API does not support detecting the interrupt pin, so we need to use
// sepparate ISR for each pin
void forwardProximityISR() {
  handleProximityISR(forwardProximitySensor);
}

/**
  * Handle the proximity sensor interrupt. The function will read the 
  * rising and falling edge of the echo pin and calculate the range.
 */
void handleProximityISR(ProximitySensor& sensor) {
  noInterrupts(); 
  uint now = micros();
  if (digitalRead(sensor.echoData.echoPin)) {  // Rising edge
    sensor.echoData.tEchoStart = now;    
  } else { // Falling edge
    sensor.echoData.tEchoEnd = now;
    uint width = now - sensor.echoData.tEchoStart;
    float range = width * SPEED_OF_SOUND_CM_US/2;
    sensor.echoData.range = range;
  }
  interrupts();
}

/**
  * Trigger the proximity sensor by sending a pulse to the trigger pin.
  * The function will return true when the trigger is complete. 
  * The function will alternate between left and righ sensors, 
  * to prevent interference.
**/
bool triggerProximitySensor() {
  ProximitySensor* sensor;
  switch (triggerState) {
    case TRIGGER_LEFT:
      sensor = &leftProximitySensor;
      triggerState = TRIGGER_RIGHT;
      break;
    case TRIGGER_RIGHT:
      sensor = &rightProximitySensor;
      triggerState = TRIGGER_FORWARD;
      break;
    case TRIGGER_FORWARD:
      sensor = &forwardProximitySensor;
      triggerState = TRIGGER_LEFT;
      break;
  }

  uint now = micros();

  if (!sensor->triggerData.isTriggered) {
    digitalWrite(sensor->triggerData.triggerPin, HIGH);
    sensor->triggerData.tTriggered = now;
    sensor->triggerData.isTriggered = true;
  } 
  else if (now - sensor->triggerData.tTriggered >= PROXIMITY_PULSE_DURATION_US) {
    digitalWrite(sensor->triggerData.triggerPin, LOW);
    sensor->triggerData.isTriggered = false;

    return true;
  }

  return false; 
}

float getProximityRange(ProximitySensor& sensor) {
  noInterrupts();
  float range = sensor.echoData.range;
  interrupts();
  return range;
}
