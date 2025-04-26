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

void resetPID(PIDData *self) {
  for (int i = 0; i < PID_WINDOW_SIZE_SAMP; i++) {
    self->error[i] = 0.0f;
  }
  if (DEBUG) {
    Serial.println("Resetting PID");
  }
}

/**
* Calculates the next control signal from the PID controller for the given PIDData struct.
* Anti integral-windup measures:
*   1. A sliding window for the accumulated error
*   2. (Optional) Resetting the accumulated error when it crosses 0
*   3. Constraining the accumulated error when calculating the I part to [-minAccumulatedError, maxAccumulatedError]
**/
float PIDControl(PIDData *self, float actualValue, float targetValue) {
  float error = targetValue - actualValue;

  float accumulatedError = error;
  float derivative = 0.0f;

  // Sliding error window
  for(int i=1; i<PID_WINDOW_SIZE_SAMP; i++) {
    accumulatedError += self->error[i];
    self->error[i-1] = self->error[i];
  }

  self->error[PID_WINDOW_SIZE_SAMP-1] = error;
  accumulatedError*= (self->samplingPeriodMS/1000); // Riemann sum: \sum (T_s*e_i) = T_s*\sum(e_i)
  derivative = (error - self->error[PID_WINDOW_SIZE_SAMP-1])/(self->samplingPeriodMS/1000);

  // Resetting accumulated error when the error crosses 0
  if (ACCUMULATOR_RESET) {
    if (error * self->error[PID_WINDOW_SIZE_SAMP-1] < 0) {
      for (int i = 0; i < PID_WINDOW_SIZE_SAMP; i++) {
        self->error[i] = 0.0f;
      }
      accumulatedError = 0.0f;
    }
  }
  
  float P = self->kP * error;
  float I = self->kI * constrain(accumulatedError,-self->maxAccumulatedError,self->maxAccumulatedError);
  float D = self->kD * derivative;
  
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