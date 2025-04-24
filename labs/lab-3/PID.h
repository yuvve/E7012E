#ifndef PID_H
#define PID_H
  typedef struct {
    float samplingPeriodMS;
    float kP;
    float kI;
    float kD;
    float accumulatedError;
    float lastError;
    float maxAccumaltedError;
  } PIDData;
#endif