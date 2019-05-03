#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

typedef struct{
  uint16_t coeffs[N];
} poly __attribute__((aligned(32)));

void poly_add(poly *r, const poly *a, const poly *b);

void mul16(uint16_t *r, const uint16_t *a, const uint16_t *b);
void mul32(uint16_t *r, const uint16_t *a, const uint16_t *b);
void mul64(uint16_t *r, const uint16_t *a, const uint16_t *b);
void mul128(uint16_t *r, const uint16_t *a, const uint16_t *b);
void mul256(uint16_t *r, const uint16_t *a, const uint16_t *b);


void mul128_red(poly *r, const poly *a, const poly *b);
void mul256_red(poly *r, const poly *a, const poly *b);

#endif
