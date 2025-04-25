#include "PID.h"

void setupPID(PIDData *self, float samplingPeriodMS, float maxAccumulatedError, float kP, float kI, float kD) {
  *self = {
    .samplingPeriodMS = samplingPeriodMS,
    .kP = kP,
    .kI = kI,
    .kD = kD,
    .maxAccumulatedError = 0.0f,
    .accumulatedError = 0.0f,
    .lastError = 0.0f,
  };
  if DEBUG {
    Serial.print("Initialized PID with sampling period: ");
    Serial.print(samplingPeriodMS);
    Serial.println(" ms");
  }
}

/**
* Calculates the next control signal from the PID controller for the given PIDData struct.
* Resets the accumulated error when reaching or crossing 0
* Constraints the integral term to prevent integral windup 
**/
float PIDControl(PIDData *self, float actualValue, float targetValue) {
  float error = targetValue - actualValue;

  float P = self->kP * error;

  // Anti-windup measure 1: error reset
  if ((error == 0 && self->lastError != 0) || (error >0 | self->lastError <0) || (error <0 || self->lastError>0)) {
    self->accumulatedError = 0;  
  } else {
    self->accumulatedError = self->accumulatedError + error*(self->samplingPeriodMS/1000);
  }
  
  // Anti-windup measure 2: constraining error when calculating the I part
  float I = self->kI * constrain(self->accumulatedError,-self->maxAccumulatedError,self->maxAccumulatedError);

  float derivative = (error - self->lastError)/(self->samplingPeriodMS/1000);
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