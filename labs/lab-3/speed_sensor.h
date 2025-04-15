#ifndef SPEED_SENSOR_H
#define SPEED_SENSOR_H
  typedef struct {
    int t0;
    int t1;
    int pulsesSinceLastUpdate;
  } SpeedSensor;
#endif