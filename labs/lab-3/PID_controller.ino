float kP = 1; // Tunable parameters
float kI = 1;
float kD = 1;
float integral = 0; // integral should be accumulated
float last_error = 0; // last_error should be saved from the previous time the function was called
void PIDControl(targetSpeed) {
  float actualSpeed = getSpeed();
  error = targetSpeed - actualSpeed;

  P = kP * error;

  integral = integral + error;
  I = kI * integral;

  derivative = error - last_error;
  D = kD * derivative;

  PID = P + I + D;
  //Saturate the actuation between -100 and 100
  if PID > 100 {
    PID = 100;
  }
  else if PID < -100 {
    PID = -100;
  }
  last_error = error;
  return 

  
}