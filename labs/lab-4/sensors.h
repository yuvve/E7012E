#ifndef SENSORS_H
#define SENSORS_H
  typedef struct {
    float speed;
  } SensorData;
  
  typedef struct {
    uint32_t t0;
    uint32_t pulses0;
    int pulses1;
  } SpeedSensor;
  
  typedef struct {
    int t_triggered;
    int t0;
    int t1;
    float range;
  } ProximitySensorData;
#endif