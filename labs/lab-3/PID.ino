#include "PID.h"

void setupPID(PIDData *self, float samplingPeriod, float kP, float kI, float kD) {
  *self = {samplingPeriod, kP, kI, kD, 0,0};
}

float PIDControl(PIDData *self, float actualSpeed, float targetSpeed) {
  float error = targetSpeed - actualSpeed;

  float P = self->kP * error;

  self->accumulatedError = self->accumulatedError + error*self->samplingPeriod;
  float I = self->kI * self->accumulatedError;

  float derivative = (error - self->lastError)/self->samplingPeriod;
  float D = self->kD * derivative;

  self->lastError = error;
  return P + I + D;
}