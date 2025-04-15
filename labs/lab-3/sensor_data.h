#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H
  typedef struct {
    float speed;
  } SensorData;

  float getSpeed();
  float setSpeed(float speed);
#endif