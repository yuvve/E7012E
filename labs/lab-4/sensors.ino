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
  if DEBUG {
    Serial.println("Speed sensor initialized");
  }
}

void setupProximitySensor(uint triggerPin, uint echoPin) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  proximitySensorData.range = 0;
  proximitySensorData.t0 = 0;
  proximitySensorData.t1 = 0;
  proximitySensorData.t_triggered = 0;

  attachInterrupt(digitalPinToInterrupt(echoPin), proximitySensorISR, CHANGE);
  Serial.println("Proximity sensor initialized");
}

void speedSensorISR() {
  speedSensor.pulses1++;
}

void proximitySensorISR() {
  noInterrupts();
  if (digitalRead(PROXIMITY_ECHO_PIN)) {
    proximitySensorData.t0 = micros();    // Rising edge
  } else {
    proximitySensorData.t1 = micros();    // Falling edge
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
  if (!proximityTriggerFlag &&(currentTime - proximitySensorData.t_triggered >= PROXIMITY_PULSE_INTERVAL_US)) {
      proximitySensorData.t_triggered = currentTime;
      digitalWrite(PROXIMITY_TRIGGER_PIN, HIGH); // Start the pulse
      proximityTriggerFlag = true;
      if DEBUG {
        Serial.print("Trigger pulse sent at: ");
        Serial.println(currentTime);
      }
  }

  // If the pulse is HIGH, turn it LOW after 10us
  if (proximityTriggerFlag && (currentTime - proximitySensorData.t_triggered >= PROXIMITY_PULSE_DURATION_US)) {
      digitalWrite(PROXIMITY_TRIGGER_PIN, LOW); // Stop the pulse
      proximityTriggerFlag = false;
      if DEBUG {
        Serial.print("Trigger pulse stopped at: ");
        Serial.println(currentTime);
      }
  }
}

void calculateProximityRange() {
  int width = proximitySensorData.t1 - proximitySensorData.t0;
  float range = width * (SPEED_OF_SOUND_CM_US/2);
  proximitySensorData.range = range;
  if DEBUG {
    Serial.print("Proximity range: ");
    Serial.println(range);
  }
}