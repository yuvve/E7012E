#ifndef PID_H
#define PID_H
  #include "constants.h"
  typedef struct {
    float samplingPeriodMS;
    float kP;
    float kI;
    float kD;
    float maxAccumulatedError;
    float error[PID_WINDOW_SIZE_SAMP];
  } PIDData;
#endif