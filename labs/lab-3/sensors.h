#ifndef SENSORS_H
#define SENSORS_H
  typedef struct {
    float speed;
  } SensorData;
  
  typedef struct {
    int t0;
    int pulses0;
    int pulses1;
  } SpeedSensor;
#endif