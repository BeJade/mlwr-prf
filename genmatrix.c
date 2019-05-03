#include <immintrin.h>
#include <string.h>
#include "genmatrix.h"
#include "params.h"
#include "prf.h"
#include "polyvec.h"
#include "fips202.h"
#include "fips202x4.h"


void genmatrix(polyvecK *a, const unsigned char *seed)
{
  unsigned int nblocks=2;
  uint8_t buf[M][K][SHAKE128_RATE*nblocks];

  unsigned char extseed0[SYMBYTES+2];
  unsigned char extseed1[SYMBYTES+2];
  unsigned char extseed2[SYMBYTES+2];
  unsigned char extseed3[SYMBYTES+2];

  int i,j,k;

  for(i=0;i<SYMBYTES;i++)
  {
    extseed0[i] = seed[i];
    extseed1[i] = seed[i];
    extseed2[i] = seed[i];
    extseed3[i] = seed[i];
  }


  for(i=0;i<M;i+=4)
  {
    for(j=0;j<K;j+=1)
    {
      extseed0[SYMBYTES]   = i;
      extseed0[SYMBYTES+1] = j;
      extseed1[SYMBYTES]   = i+1;
      extseed1[SYMBYTES+1] = j;
      extseed2[SYMBYTES]   = i+2;
      extseed2[SYMBYTES+1] = j;
      extseed3[SYMBYTES]   = i+3;
      extseed3[SYMBYTES+1] = j;

      shake128x4(buf[i][j], buf[i+1][j], buf[i+2][j], buf[i+3][j],SHAKE128_RATE*nblocks,
        extseed0, extseed1, extseed2, extseed3, SYMBYTES+2);
    }

  }

  __m256i tmp0;

  for(i=0;i<M;i++)
  {
    for(j=0;j<K;j++)
    {
      for(k=0;k<8;k++)
      {
        tmp0 = _mm256_loadu_si256((__m256i *)&buf[i][j][k*32]);
        _mm256_storeu_si256((__m256i *)&a[i].vec[j].coeffs[k*16], tmp0);
      }
    }

  }

  return;

}

