#include <immintrin.h>
#include <string.h>

#include "cpucycles.h"
#include "randombytes.h"
#include "fips202.h"
#include "poly.h"
#include "polyvec.h"
#include "genmatrix.h"
#include "params.h"
#include "prf.h"

#include<stdio.h>

extern long long cpucycles(void);
int64_t selectedcycles = 0;

polyvecK a[M];

const char *byte_to_binary(uint8_t x)
{
	static char b[18];
	b[0] = '\0';
	int z;
	for (z=128; z > 0; z>>=1)
	{
		strcat(b, ((x & z) == z) ? " 1" : " 0");
		if (z == 16) strcat(b, "\t");
	}
	return b;
}

const char *byte2_to_binary(uint16_t x)
{
	static char b[36];
	b[0] = '\0';
	int z;
	for (z=32768; z > 0; z>>=1)
	{
		strcat(b, ((x & z) == z) ? " 1" : " 0");
		if (z == 16) strcat(b, "\t");
		if (z == 256) strcat(b, "\t");
		if (z == 4096) strcat(b, "\t");
	}
	return b;
}

void mv_mul(poly *t, polyvecK *s, int c)
{
	poly r;
	mul256_red(t, &a[c].vec[0], &s->vec[0]);

	for (int i=1; i<K; i++)
	{
		mul128_red(&r, &a[c].vec[i], &s->vec[i]);
		poly_add(t, t, &r);
	}
}

void extractS(polyvecK *s, poly *t)
{

	__m256i tmp0, tmp1, tmp2;
	const __m256i mask0xf = _mm256_set1_epi16(0xf);
	const __m256i neg8 = _mm256_set1_epi16(-8);

	// assume K=3
	for (int i=0; i<16; i+=1)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&t->coeffs[i*16]);

		tmp0 = _mm256_srli_epi16(tmp0,4);
		tmp1 = _mm256_and_si256(tmp0, mask0xf);

		tmp2 = _mm256_srli_epi16(tmp1, 3);
		tmp2 = _mm256_add_epi16(tmp1,tmp2);
		tmp2 = _mm256_add_epi16(tmp2,neg8);

		_mm256_storeu_si256((__m256i *)&s->vec[0].coeffs[i*16], tmp2);

	
		tmp0 = _mm256_srli_epi16(tmp0,4);
		tmp1 = _mm256_and_si256(tmp0, mask0xf);

		tmp2 = _mm256_srli_epi16(tmp1, 3);
		tmp2 = _mm256_add_epi16(tmp1,tmp2);
		tmp2 = _mm256_add_epi16(tmp2,neg8);

		_mm256_storeu_si256((__m256i *)&s->vec[1].coeffs[i*16], tmp2);

	
		tmp1 = _mm256_srli_epi16(tmp0,4);
		//tmp1 = _mm256_and_si256(tmp0, mask0xf);

		tmp2 = _mm256_srli_epi16(tmp1, 3);
		tmp2 = _mm256_add_epi16(tmp1,tmp2);
		tmp2 = _mm256_add_epi16(tmp2,neg8);

		_mm256_storeu_si256((__m256i *)&s->vec[2].coeffs[i*16], tmp2);
	}
}

void extractOut(unsigned char *out, poly *t)
{
	__m256i tmp0, tmp1, tmp2;
	const __m256i mask0xf = _mm256_set1_epi16(0xf);
	const __m256i mask0x12 = _mm256_set1_epi16(0xfff0);

	for (int i=0,j=0; i<16; i+=4,j+=3)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&t->coeffs[i*16]);

		//--

		tmp0 = _mm256_srli_epi16(tmp0,4);
		tmp1 = _mm256_and_si256(tmp1, mask0xf);

		tmp2 = _mm256_loadu_si256((__m256i *)&t->coeffs[(i+1)*16]);
		tmp2 = _mm256_and_si256(tmp2, mask0x12);

		tmp1 = _mm256_add_epi16(tmp1,tmp2);
		_mm256_storeu_si256((__m256i *)&out[j*32], tmp1);

		//--

		tmp0 = _mm256_srli_epi16(tmp0,4);
		tmp1 = _mm256_and_si256(tmp1, mask0xf);

		tmp2 = _mm256_loadu_si256((__m256i *)&t->coeffs[(i+2)*16]);
		tmp2 = _mm256_and_si256(tmp2, mask0x12);

		tmp1 = _mm256_add_epi16(tmp1,tmp2);
		_mm256_storeu_si256((__m256i *)&out[(j+1)*32], tmp1);

		//--

		tmp1 = _mm256_srli_epi16(tmp0,4);
		//tmp1 = _mm256_and_si256(tmp1, mask0xf);

		tmp2 = _mm256_loadu_si256((__m256i *)&t->coeffs[(i+3)*16]);
		tmp2 = _mm256_and_si256(tmp2, mask0x12);

		tmp1 = _mm256_add_epi16(tmp1,tmp2);
		_mm256_storeu_si256((__m256i *)&out[(j+2)*32], tmp1);

	}
}

void precompute(polyvecK *s, const unsigned char *seed)
{
	unsigned int nblocks = 3;
	uint8_t buf1[SHAKE128_RATE * nblocks];

	shake128(buf1, SHAKE128_RATE*nblocks, seed, SEEDBYTES);

	__m256i tmp00,tmp01,tmp02,tmp03,tmp04,tmp11,tmp12,tmp13,tmp14;
	const __m256i mask0xf = _mm256_set1_epi16(0xf);
	const __m256i neg8 = _mm256_set1_epi16(-8);

	// each coeff of s is 4 bits
	for (int i=0; i<K; i++)
	{
		for (int j=0; j<16; j+=4)
		{
			tmp00 = _mm256_loadu_si256((__m256i *)&buf1[i*256 + j*8]);

			tmp01 = _mm256_and_si256(tmp00, mask0xf);

			tmp02 = _mm256_srli_epi16(tmp00, 4);
			tmp02 = _mm256_and_si256(tmp02, mask0xf);

			tmp03 = _mm256_srli_epi16(tmp00, 8);
			tmp03 = _mm256_and_si256(tmp03, mask0xf);

			tmp04 = _mm256_srli_epi16(tmp00, 12);
			tmp04 = _mm256_and_si256(tmp04, mask0xf);


			tmp11 = _mm256_srli_epi16(tmp01, 3);
			tmp11 = _mm256_add_epi16(tmp11,tmp01);
			tmp11 = _mm256_add_epi16(tmp11,neg8);

			tmp12 = _mm256_srli_epi16(tmp02, 3);
			tmp12 = _mm256_add_epi16(tmp12,tmp02);
			tmp12 = _mm256_add_epi16(tmp12,neg8);

			tmp13 = _mm256_srli_epi16(tmp03, 3);
			tmp13 = _mm256_add_epi16(tmp13,tmp03);
			tmp13 = _mm256_add_epi16(tmp13,neg8);

			tmp14 = _mm256_srli_epi16(tmp04, 3);
			tmp14 = _mm256_add_epi16(tmp14,tmp04);
			tmp14 = _mm256_add_epi16(tmp14,neg8);


			_mm256_storeu_si256((__m256i *)&s->vec[i].coeffs[i*16 + j], tmp11);
			_mm256_storeu_si256((__m256i *)&s->vec[i].coeffs[i*16 + j+1], tmp12);
			_mm256_storeu_si256((__m256i *)&s->vec[i].coeffs[i*16 + j+2], tmp12);
			_mm256_storeu_si256((__m256i *)&s->vec[i].coeffs[i*16 + j+3], tmp12);
		}
	}

}

void mainloop(unsigned char *out, polyvecK *s, const unsigned char *bits)
{
	poly t;

	uint8_t tblC[DEPTH+1];
	polyvecK tblS[DEPTH+1];

	int idx=1;

	tblS[0] = *s;
	tblC[0] = 0;


	int pos=0;
	for (int l=0,shf=0; l<LENGTH; l+=4)
	{
		// select rows
		int c = (*(bits + pos) >> shf) & 0xF;
		shf = (shf + 4) & 0x7;
		pos += (shf>>2);
		tblC[idx] = c;

		mv_mul(&t, s, c);
		extractS(s,&t);

		tblS[idx++] = *s;
	}

	// output
	for (int i=0; i<OMEGA; i++)
	{
		mv_mul(&t, s, i);
		extractOut(&out[i*OUT_PER_I],&t);	// 384 = 256 * 12 / 8
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////

	int l = DEPTH;
	for (int r=1; r<ROUND; r++)
	{
		//---- update c (c+1) ----//
		tblC[l] = tblC[l] + 1;
		int currC = tblC[l];
		while ((currC &0xF) == 0)	// currC & (OMEGA-1)
		{
			tblC[l] = 0;
			l -= 1;

			if (l < 1) { printf("ERROR: l < 1\n"); return ; }

			tblC[l] = tblC[l] + 1;
			currC = tblC[l];
		}

		for (; l<=DEPTH; l++)
		{
			*s = tblS[l-1];
			int c = tblC[l];

			mv_mul(&t, s, c);
			extractS(s,&t);

			tblS[l] = *s;
		}


		//---- l == DEPTH ----//
		*s = tblS[DEPTH];
		for (int i=0; i<OMEGA; i++)
		{
			mv_mul(&t, s, i);
			extractOut(&out[r*OMEGA*OUT_PER_I + i*OUT_PER_I], &t);
		}
	}

}

void setup()
{
	unsigned char buf0[SYMBYTES + SYMBYTES];
	unsigned char *publicseed = buf0;

	randombytes(buf0, SYMBYTES);
	sha3_512(buf0, buf0, SYMBYTES);

	genmatrix(&a[0], publicseed);
}

void prflwr(unsigned char *out, const unsigned char *seed, const unsigned char *bits)
{
selectedcycles = 0;
	setup();
	polyvecK s;
selectedcycles -= cpucycles();
	precompute(&s, seed);
	mainloop(out,&s,bits);
selectedcycles += cpucycles();
}
