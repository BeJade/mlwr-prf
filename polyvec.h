#ifndef POLYVEC_H
#define POLYVEC_H

#include "params.h"
#include "poly.h"

typedef struct{
  poly vec[K];
} polyvecK;

typedef struct{
  poly vec[M];
} polyvecM;


#endif
