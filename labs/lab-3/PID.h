#ifndef PID_H
#define PID_H
  typedef struct {
    float samplingPeriod;
    float kP;
    float kI;
    float kD;
    float accumulatedError;
    float lastError;
  } PIDData;
#endif