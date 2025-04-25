#ifndef PID_H
#define PID_H
  typedef struct {
    float samplingPeriodMS;
    float kP;
    float kI;
    float kD;
    float maxAccumulatedError;
    float accumulatedError;
    float lastError;
  } PIDData;
#endif