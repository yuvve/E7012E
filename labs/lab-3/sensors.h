#ifndef SENSORS_H
#define SENSORS_H
  typedef struct {
    float speed;
  } SensorData;
  
  typedef struct {
    int t0;
    int t1;
    int pulses;
  } SpeedSensor;
#endif