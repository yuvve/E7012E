#ifndef slidingMode_H
#define slidingMode_H
  #include "constants.h"
  typedef struct {
    float T;
    float epsilon;
    float lambda;
    float p[2];
    float sigma;
  } slidingMode;
#endif