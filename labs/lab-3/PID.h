#ifndef PID_H
#define PID_H
  typedef struct {
    float kP; // Tunable parameters
    float kI;
    float kD;
    float integral; // integral should be accumulated
    float last_error; // last_error should be saved from the previous time the function was called
  } PIDData;
#endif