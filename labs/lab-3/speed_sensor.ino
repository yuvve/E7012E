int t0;
int t1;
int pulsesSinceLastUpdate = 0;

void setupSpeedSensor(int pin, SensorData sensor_data) {
  t1 = micros();
  attachInterrupt(digitalPinToInterrupt(pin), speedSensorISR, RISING); 
  if DEBUG {
    Serial.println("Speed sensor initialized");
  }
}

void speedSensorISR() {
  pulsesSinceLastUpdate++;
  if (pulsesSinceLastUpdate % 4) {
    t0 = t1;
    t1 = micros();
    pulsesSinceLastUpdate = 0;
  }
}

void updateSpeed(int t0, int t1, int nrPulses) {
  sensorData.speed = calc_speed(t0, t1, nrPulses);
}

float calc_speed(int t0, int t1, int nrPulses) {
  return (t1-t0)/nrPulses;
}