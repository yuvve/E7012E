#include "PID.h"

void setupPID(PIDData *self, float samplingPeriodMS, float kP, float kI, float kD) {
  *self = {samplingPeriodMS, kP, kI, kD, 0.0f,0.0f};
  if DEBUG {
    Serial.print("Initialized PID with sampling period: ");
    Serial.print(samplingPeriodMS);
    Serial.println(" ms");
  }
}

float PIDControl(PIDData *self, float actualSpeed, float targetSpeed) {
  float error = targetSpeed - actualSpeed;

  float P = self->kP * error;

  self->accumulatedError = self->accumulatedError + error*self->samplingPeriodMS;
  float I = self->kI * self->accumulatedError;

  float derivative = (error - self->lastError)/self->samplingPeriodMS;
  float D = self->kD * derivative;

  self->lastError = error;
  return P + I + D;
}

void adjustP(PIDData *self, float kP) {
  self->kP = kP;
  if (DEBUG) {
    Serial.print("Changing PID kP to: ");
    Serial.println(kP);
  }
}

void adjustI(PIDData *self, float kI) {
  self->kI = kI;
  if (DEBUG) {
    Serial.print("Changing PID kI to: ");
    Serial.println(kI);
  }
}

void adjustD(PIDData *self, float kD) {
  self->kD = kD;
    if (DEBUG) {
    Serial.print("Changing PID kD to: ");
    Serial.println(kD);
  }
}