#include "PID.h"

void setupPID(PIDData *self, float samplingPeriodMS, float maxAccumulatedError, float kP, float kI, float kD) {
  *self = {
    .samplingPeriodMS = samplingPeriodMS,
    .kP = kP,
    .kI = kI,
    .kD = kD,
    .maxAccumulatedError = 0.0f,
    .error = {0.0f},
  };
  if DEBUG {
    Serial.print("Initialized PID with sampling period: ");
    Serial.print(samplingPeriodMS);
    Serial.println(" ms");
  }
}

/**
* Calculates the next control signal from the PID controller for the given PIDData struct.
* Uses a sliding window for the accumulated error
* Constraints the integral term to prevent integral windup 
**/
float PIDControl(PIDData *self, float actualValue, float targetValue) {
  float error = targetValue - actualValue;

  // Anti-windup measure 1: sliding error window
  float accumulatedError = error;
  for(int i=1; i<PID_WINDOW_SIZE_SAMP; i++) {
    accumulatedError += self->error[i];
    self->error[i-1] = self->error[i];
  }

  self->error[PID_WINDOW_SIZE_SAMP-1] = error;
  
  float P = self->kP * error;
  // Anti-windup measure 2: constraining error when calculating the I part
  float I = self->kI * constrain(accumulatedError,-self->maxAccumulatedError,self->maxAccumulatedError);
  float D = self->kD * (error - self->error[PID_WINDOW_SIZE_SAMP-1])/(self->samplingPeriodMS/1000);
  
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