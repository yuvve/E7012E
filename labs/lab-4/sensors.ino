#include "sensors.h"

SensorData sensorData;
SpeedSensor speedSensor;
volatile ProximitySensorData proximitySensorData;

void setupSensorData() {
  sensorData = {0.0f};
}

void setupSpeedSensor(unsigned int pin) {
  pinMode(pin, INPUT);
  speedSensor = {micros(), 0, 0};
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  DEBUG_PRINTLN("Speed sensor initialized");
}

void setupProximitySensor(uint triggerPin, uint echoPin) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  proximitySensorData.range = 0;
  proximitySensorData.tEchoStart = 0;
  proximitySensorData.tEchoEnd = 0;
  proximitySensorData.tTriggered = 0;

  attachInterrupt(digitalPinToInterrupt(echoPin), proximitySensorISR, CHANGE);
  DEBUG_PRINTLN("Proximity sensor initialized");
}

void speedSensorISR() {
  speedSensor.pulses1++;
}

void proximitySensorISR() {
  noInterrupts();
  if (digitalRead(PROXIMITY_ECHO_PIN)) {
    proximitySensorData.tEchoStart = micros();    // Rising edge
  } else {
    proximitySensorData.tEchoEnd = micros();    // Falling edge
    proximityEchoFlag = true;
  }
  interrupts();
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

void sendProximityPulse() {
  uint currentTime = micros();

  // Check if it is time to send another pulse
  if (!proximityTriggerFlag &&(currentTime - proximitySensorData.tTriggered >= PROXIMITY_PULSE_INTERVAL_US)) {
      proximitySensorData.tTriggered = currentTime;
      digitalWrite(PROXIMITY_TRIGGER_PIN, HIGH); // Start the pulse
      proximityTriggerFlag = true;
      DEBUG_PRINTF("Trigger pulse sent at: %d", currentTime);
  }

  // If the pulse is HIGH, turn it LOW after 10us
  if (proximityTriggerFlag && (currentTime - proximitySensorData.tTriggered >= PROXIMITY_PULSE_DURATION_US)) {
      digitalWrite(PROXIMITY_TRIGGER_PIN, LOW); // Stop the pulse
      proximityTriggerFlag = false;
      DEBUG_PRINTF("Trigger pulse stopped at: %d", currentTime);
  }
}

void calculateProximityRange() {
  int width = proximitySensorData.tEchoEnd - proximitySensorData.tEchoStart;
  float range = width * (SPEED_OF_SOUND_CM_US/2);
  proximitySensorData.range = range;
  DEBUG_PRINTF("Proximity range: %f cm", range);
}