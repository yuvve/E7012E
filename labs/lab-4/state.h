#ifndef state_H
#define state_H
  #include "constants.h"
  typedef struct {
    float theta;
    float error;
    float alpha;
    float speed;
    float bredd;
    float korridorBredd;
    float hjulbas;
  } state;
#endif