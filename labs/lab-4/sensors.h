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
    uint triggerPin;
    bool isTriggered;
    uint tTriggered;
  } TriggerData;

  typedef struct {
    uint echoPin;
    uint tEchoStart;
    uint tEchoEnd;
    float range;
  } EchoData;
  
  typedef struct {
    TriggerData triggerData;
    EchoData echoData;
  } ProximitySensor;
#endif