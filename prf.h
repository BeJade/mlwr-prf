#ifndef PRF_H
#define PRF_H

#include "polyvec.h"

void prflwr(unsigned char *out, const unsigned char seed[SEEDBYTES], const unsigned char *bits);

void precompute(polyvecK *s, const unsigned char *seed);
void mainloop(unsigned char *out, polyvecK *s, const unsigned char *bits);


#endif
