#include "PID.h"

PIDData pidData;

void setupPID(float kP, float kI, float kD) {
  pidData = {kP, kI, kD, 0,0};
}

float PIDControl(float actualSpeed, float targetSpeed) {
  float error = targetSpeed - actualSpeed;

  float P = pidData.kP * error;

  pidData.integral = pidData.integral + error;
  float I = pidData.kI * pidData.integral;

  float derivative = error - pidData.last_error;
  float D = pidData.kD * derivative;

  float PID = P + I + D;

  pidData.last_error = error;
  return PID;
}