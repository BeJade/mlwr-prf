//#include "api.h"
#include "params.h"
#include "prf.h"
#include "poly.h"
#include "polyvec.h"
#include "genmatrix.h"
#include "cpucycles.h"
#include <stdlib.h>
#include <stdio.h>

#define NTESTS 10000

unsigned long long tsel[NTESTS];
extern long long selectedcycles;

static int cmp_llu(const void *a, const void*b)
{
  if(*(unsigned long long *)a < *(unsigned long long *)b) return -1;
  if(*(unsigned long long *)a > *(unsigned long long *)b) return 1;
  return 0;
}

static unsigned long long median(unsigned long long *l, size_t llen)
{
  qsort(l,llen,sizeof(unsigned long long),cmp_llu);

  if(llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}

static unsigned long long average(unsigned long long *t, size_t tlen)
{
  unsigned long long acc=0;
  size_t i;
  for(i=0;i<tlen;i++)
    acc += t[i];
  return acc/(tlen);
}

static void print_results(const char *s, unsigned long long *t, size_t tlen)
{
  size_t i;
  printf("%s", s);
  for(i=0;i<tlen-1;i++)
  {
    t[i] = t[i+1] - t[i];
  }
  printf("\n");
  printf("median: %llu\n", median(t, tlen));
  printf("average: %llu\n", average(t, tlen-1));
  printf("selected: %llu\n", median(tsel, tlen));
  printf("\n");
}


unsigned long long t[NTESTS];
unsigned char seed[32] = {0};

int main()
{
  int i;

  static unsigned char out[OUT_PER_I * OMEGA * ROUND];
  const unsigned char seed[SEEDBYTES];
  unsigned char bits[LENGTH/8];

  for (i=0; i<LENGTH/8; i++)
    bits[i] = 0x00;


  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    prflwr(out,seed,bits);
    tsel[i] = selectedcycles;
  }
  print_results("prf:  ", t, NTESTS);

  return 0;
}
