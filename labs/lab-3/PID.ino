#include "PID.h"

PIDData pidData;

void setupPID(float samplingPeriod, float kP, float kI, float kD) {
  pidData = {samplingPeriod, kP, kI, kD, 0,0};
}

float PIDControl(float actualSpeed, float targetSpeed) {
  float error = targetSpeed - actualSpeed;

  float P = pidData.kP * error;
  
  pidData.accumulatedError = pidData.accumulatedError + error*samplingPeriod;
  float I = pidData.kI * pidData.accumulatedError;

  float derivative = (error - pidData.lastError)/samplingPeriod;
  float D = pidData.kD * derivative;

  pidData.lastError = error;
  return P + I + D;
}