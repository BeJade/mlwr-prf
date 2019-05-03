#include <stdio.h>
#include <immintrin.h>
#include "poly.h"

const uint16_t mask[] = {
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,	0xffff,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xffff,

	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0,	0x0,
	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0xffff,	0x0

};

void poly_add(poly *r, const poly *a, const poly *b)
{
	__m256i tmp0, tmp1;

	for (int i=0; i<N; i+=16)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&a->coeffs[i]);
		tmp1 = _mm256_loadu_si256((__m256i *)&b->coeffs[i]);
		tmp0 = _mm256_add_epi16(tmp0,tmp1);
		_mm256_storeu_si256((__m256i *)&r->coeffs[i], tmp0);
	}
}

void mul16(uint16_t *r, const uint16_t *a, const uint16_t *b)
{
	__m256i tmp0,tmp1,tmp2;
	__m256i tmpa[16],tmpb[16];

	__m256i tmp00 = _mm256_set1_epi16(0x0);
	__m256i tmp01 = _mm256_set1_epi16(0x0);

	tmp0 = _mm256_loadu_si256((__m256i *)&a[0]);
	tmp1 = _mm256_set1_epi16(b[0]);

	tmpa[0] = _mm256_mullo_epi16(tmp0,tmp1);
	tmp00 = _mm256_add_epi16(tmp00,tmpa[0]);

	for (int i=1; i<16; i++)
	{
		tmp1 = _mm256_slli_epi64(tmp0,16);
		tmp2 = _mm256_srli_epi64(tmp0,48);

		tmp2 = _mm256_permute4x64_epi64(tmp2,0x93);
		tmp0 = _mm256_add_epi16(tmp1,tmp2);
		tmp1 = _mm256_set1_epi16(b[i]);
		tmpa[i] = _mm256_mullo_epi16(tmp0,tmp1);

		tmp1 = _mm256_loadu_si256((__m256i *)&mask[(16+i)*16]);
		tmpb[i] = _mm256_and_si256(tmpa[i], tmp1);

		tmp1 = _mm256_loadu_si256((__m256i *)&mask[i*16]);
		tmpa[i] = _mm256_and_si256(tmpa[i], tmp1);

		tmp00 = _mm256_add_epi16(tmp00,tmpa[i]);
		tmp01 = _mm256_add_epi16(tmp01,tmpb[i]);
	}

	_mm256_storeu_si256((__m256i *)&r[0], tmp00);
	_mm256_storeu_si256((__m256i *)&r[16], tmp01);
}


void mul32(uint16_t *r, const uint16_t *a, const uint16_t *b)
{
	__m256i tmp0,tmp1,tmp2;

	uint16_t v0[16],v1[16];
	uint16_t vr0[32],vr1[32],vr2[32];

	mul16(vr0, &a[0], &b[0]);
	mul16(vr1, &a[16], &b[16]);

	tmp0 = _mm256_loadu_si256((__m256i *)&a[0]);
	tmp1 = _mm256_loadu_si256((__m256i *)&a[16]);
	tmp1 = _mm256_add_epi16(tmp0,tmp1);
	_mm256_storeu_si256((__m256i *)&v0, tmp1);

	tmp0 = _mm256_loadu_si256((__m256i *)&b[0]);
	tmp2 = _mm256_loadu_si256((__m256i *)&b[16]);
	tmp2 = _mm256_add_epi16(tmp0,tmp2);
	_mm256_storeu_si256((__m256i *)&v1, tmp2);

	mul16(vr2, v0, v1);

	__m256i tmp00,tmp01,tmp10,tmp11,tmp20,tmp21;

	tmp00 = _mm256_loadu_si256((__m256i *)&vr0[0]);
	tmp01 = _mm256_loadu_si256((__m256i *)&vr0[16]);
	tmp10 = _mm256_loadu_si256((__m256i *)&vr1[0]);
	tmp11 = _mm256_loadu_si256((__m256i *)&vr1[16]);
	tmp20 = _mm256_loadu_si256((__m256i *)&vr2[0]);
	tmp21 = _mm256_loadu_si256((__m256i *)&vr2[16]);

	// A
	_mm256_storeu_si256((__m256i *)&r[0], tmp00);

	// B-C -A +E
	tmp0 = _mm256_sub_epi16(tmp01,tmp10);
	tmp0 = _mm256_sub_epi16(tmp0,tmp00);
	tmp0 = _mm256_add_epi16(tmp0,tmp20);
	_mm256_storeu_si256((__m256i *)&r[16], tmp0);

	// C-B -D +F
	tmp1 = _mm256_sub_epi16(tmp10,tmp01);
	tmp1 = _mm256_sub_epi16(tmp1,tmp11);
	tmp1 = _mm256_add_epi16(tmp1,tmp21);
	_mm256_storeu_si256((__m256i *)&r[32], tmp1);

	// D
	_mm256_storeu_si256((__m256i *)&r[48], tmp11);
}


void mul64(uint16_t *r, const uint16_t *a, const uint16_t *b)
{
	__m256i tmp0,tmp1;
	__m256i tmp00,tmp01,tmp10,tmp11,tmp20,tmp21;

	uint16_t v0[32],v1[32];
	uint16_t vr0[64],vr1[64],vr2[64];

	mul32(vr0,&a[0],&b[0]);
	mul32(vr1,&a[32],&b[32]);

	tmp0 = _mm256_loadu_si256((__m256i *)&a[0]);
	tmp1 = _mm256_loadu_si256((__m256i *)&a[32]);
	tmp00 = _mm256_add_epi16(tmp0,tmp1);
	_mm256_storeu_si256((__m256i *)&v0[0], tmp00);

	tmp0 = _mm256_loadu_si256((__m256i *)&a[16]);
	tmp1 = _mm256_loadu_si256((__m256i *)&a[48]);
	tmp01 = _mm256_add_epi16(tmp0,tmp1);
	_mm256_storeu_si256((__m256i *)&v0[16], tmp01);

	tmp0 = _mm256_loadu_si256((__m256i *)&b[0]);
	tmp1 = _mm256_loadu_si256((__m256i *)&b[32]);
	tmp10 = _mm256_add_epi16(tmp0,tmp1);
	_mm256_storeu_si256((__m256i *)&v1[0], tmp10);

	tmp0 = _mm256_loadu_si256((__m256i *)&b[16]);
	tmp1 = _mm256_loadu_si256((__m256i *)&b[48]);
	tmp11 = _mm256_add_epi16(tmp0,tmp1);
	_mm256_storeu_si256((__m256i *)&v1[16], tmp11);

	mul32(vr2, v0, v1);

	////////////////////

	__m256i tmpA0,tmpA1,tmpB0,tmpB1,tmpC0,tmpC1,tmpD0,tmpD1,tmpE0,tmpE1,tmpF0,tmpF1;

	// A
	tmpA0 = _mm256_loadu_si256((__m256i *)&vr0[0]);		// A0
	tmpA1 = _mm256_loadu_si256((__m256i *)&vr0[16]);	// A1

	// B-C -A +E
	tmpB0 = _mm256_loadu_si256((__m256i *)&vr0[32]);	// B0
	tmpC0 = _mm256_loadu_si256((__m256i *)&vr1[0]);		// C0
	tmp10 = _mm256_sub_epi16(tmpB0,tmpC0);

	tmp10 = _mm256_sub_epi16(tmp10,tmpA0);

	tmpE0 = _mm256_loadu_si256((__m256i *)&vr2[0]);		// E0
	tmp10 = _mm256_add_epi16(tmp10,tmpE0);


	tmpB1 = _mm256_loadu_si256((__m256i *)&vr0[32+16]);	// B1
	tmpC1 = _mm256_loadu_si256((__m256i *)&vr1[16]);	// C1
	tmp11 = _mm256_sub_epi16(tmpB1,tmpC1);

	tmp11 = _mm256_sub_epi16(tmp11,tmpA1);

	tmpE1 = _mm256_loadu_si256((__m256i *)&vr2[16]);	// E1
	tmp11 = _mm256_add_epi16(tmp11,tmpE1);

	// C-B -D +F
	tmp20 = _mm256_sub_epi16(tmpC0,tmpB0);

	tmpD0 = _mm256_loadu_si256((__m256i *)&vr1[32]);	// D0
	tmp20 = _mm256_sub_epi16(tmp20,tmpD0);

	tmpF0 = _mm256_loadu_si256((__m256i *)&vr2[32]);	// F0
	tmp20 = _mm256_add_epi16(tmp20,tmpF0);


	tmp21 = _mm256_sub_epi16(tmpC1,tmpB1);

	tmpD1 = _mm256_loadu_si256((__m256i *)&vr1[32+16]);	// D1
	tmp21 = _mm256_sub_epi16(tmp21,tmpD1);

	tmpF1 = _mm256_loadu_si256((__m256i *)&vr2[32+16]);	// F1
	tmp21 = _mm256_add_epi16(tmp21,tmpF1);

	// D
	_mm256_storeu_si256((__m256i *)&r[16*0], tmpA0);
	_mm256_storeu_si256((__m256i *)&r[16*1], tmpA1);

	_mm256_storeu_si256((__m256i *)&r[16*2], tmp10);
	_mm256_storeu_si256((__m256i *)&r[16*3], tmp11);

	_mm256_storeu_si256((__m256i *)&r[16*4], tmp20);
	_mm256_storeu_si256((__m256i *)&r[16*5], tmp21);

	_mm256_storeu_si256((__m256i *)&r[16*6], tmpD0);
	_mm256_storeu_si256((__m256i *)&r[16*7], tmpD1);
}


void mul128(uint16_t *r, const uint16_t *a, const uint16_t *b)
{
	__m256i tmp0,tmp1;

	uint16_t v0[64],v1[64];
	uint16_t vr0[128],vr1[128],vr2[128];

	mul64(vr0,&a[0],&b[0]);
	mul64(vr1,&a[64],&b[64]);

	for (int i=0; i<4; i++)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&a[i*16]);
		tmp1 = _mm256_loadu_si256((__m256i *)&a[i*16+64]);
		tmp1 = _mm256_add_epi16(tmp0,tmp1);
		_mm256_storeu_si256((__m256i *)&v0[i*16], tmp1);

		tmp0 = _mm256_loadu_si256((__m256i *)&b[i*16]);
		tmp1 = _mm256_loadu_si256((__m256i *)&b[i*16+64]);
		tmp1 = _mm256_add_epi16(tmp0,tmp1);
		_mm256_storeu_si256((__m256i *)&v1[i*16], tmp1);
	}

	mul64(vr2, v0, v1);

	//////////////////////////////

	__m256i tmpA[4],tmpB[4],tmpC[4],tmpD[4],tmpE[4],tmpF[4],tmp2[4],tmp3[4];

	for (int i=0; i<4; i++)
	{
		tmpA[i] = _mm256_loadu_si256((__m256i *)&vr0[i*16]);
		tmpB[i] = _mm256_loadu_si256((__m256i *)&vr0[i*16+64]);
		tmpC[i] = _mm256_loadu_si256((__m256i *)&vr1[i*16]);
		tmpD[i] = _mm256_loadu_si256((__m256i *)&vr1[i*16+64]);
		tmpE[i] = _mm256_loadu_si256((__m256i *)&vr2[i*16]);
		tmpF[i] = _mm256_loadu_si256((__m256i *)&vr2[i*16+64]);
	}

	for (int i=0; i<4; i++)
	{
		// A
		_mm256_storeu_si256((__m256i *)&r[i*16], tmpA[i]);

		// B-C -A +E
		tmp2[i] = _mm256_sub_epi16(tmpB[i],tmpC[i]);
		tmp2[i] = _mm256_sub_epi16(tmp2[i],tmpA[i]);
		tmp2[i] = _mm256_add_epi16(tmp2[i],tmpE[i]);

		_mm256_storeu_si256((__m256i *)&r[(i+4)*16], tmp2[i]);

		// C-B -D +F
		tmp3[i] = _mm256_sub_epi16(tmpC[i],tmpB[i]);
		tmp3[i] = _mm256_sub_epi16(tmp3[i],tmpD[i]);
		tmp3[i] = _mm256_add_epi16(tmp3[i],tmpF[i]);

		_mm256_storeu_si256((__m256i *)&r[(i+8)*16], tmp3[i]);

		// D
		_mm256_storeu_si256((__m256i *)&r[(i+12)*16], tmpD[i]);
	}
}


void mul128_red(poly *r, const poly *a, const poly *b)
{
	uint16_t vr[256];
	mul128(vr,&a->coeffs[0],&b->coeffs[0]);

	__m256i tmp0,tmp1;

	for (int i=0; i<8; i++)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&vr[i*16]);
		tmp1 = _mm256_loadu_si256((__m256i *)&vr[i*16+128]);
		tmp0 = _mm256_sub_epi16(tmp0,tmp1);

		_mm256_storeu_si256((__m256i *)&r->coeffs[i*16], tmp0);
	}
}


void mul256(uint16_t *r, const uint16_t *a, const uint16_t *b)
{
	__m256i tmp0,tmp1;

	uint16_t v0[128],v1[128];
	uint16_t vr0[256],vr1[256],vr2[256];

	mul128(vr0,&a[0],&b[0]);
	mul128(vr1,&a[128],&b[128]);

	for (int i=0; i<8; i++)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&a[i*16]);
		tmp1 = _mm256_loadu_si256((__m256i *)&a[i*16+128]);
		tmp1 = _mm256_add_epi16(tmp0,tmp1);
		_mm256_storeu_si256((__m256i *)&v0[i*16], tmp1);

		tmp0 = _mm256_loadu_si256((__m256i *)&b[i*16]);
		tmp1 = _mm256_loadu_si256((__m256i *)&b[i*16+128]);
		tmp1 = _mm256_add_epi16(tmp0,tmp1);
		_mm256_storeu_si256((__m256i *)&v1[i*16], tmp1);
	}

	mul128(vr2, v0, v1);

	//////////////////////////////

	__m256i tmpA[8],tmpB[8],tmpC[8],tmpD[8],tmpE[8],tmpF[8],tmp2[8],tmp3[8];

	for (int i=0; i<8; i++)
	{
		tmpA[i] = _mm256_loadu_si256((__m256i *)&vr0[i*16]);
		tmpB[i] = _mm256_loadu_si256((__m256i *)&vr0[i*16+128]);
		tmpC[i] = _mm256_loadu_si256((__m256i *)&vr1[i*16]);
		tmpD[i] = _mm256_loadu_si256((__m256i *)&vr1[i*16+128]);
		tmpE[i] = _mm256_loadu_si256((__m256i *)&vr2[i*16]);
		tmpF[i] = _mm256_loadu_si256((__m256i *)&vr2[i*16+128]);
	}

	for (int i=0; i<8; i++)
	{

		// A
		_mm256_storeu_si256((__m256i *)&r[i*16], tmpA[i]);

		// B-C -A +E
		tmp2[i] = _mm256_sub_epi16(tmpB[i],tmpC[i]);
		tmp2[i] = _mm256_sub_epi16(tmp2[i],tmpA[i]);
		tmp2[i] = _mm256_add_epi16(tmp2[i],tmpE[i]);

		_mm256_storeu_si256((__m256i *)&r[(i+8)*16], tmp2[i]);

		// C-B -D +F
		tmp3[i] = _mm256_sub_epi16(tmpC[i],tmpB[i]);
		tmp3[i] = _mm256_sub_epi16(tmp3[i],tmpD[i]);
		tmp3[i] = _mm256_add_epi16(tmp3[i],tmpF[i]);

		_mm256_storeu_si256((__m256i *)&r[(i+16)*16], tmp3[i]);

		// D
		_mm256_storeu_si256((__m256i *)&r[(i+24)*16], tmpD[i]);
	}
}



void mul256_red(poly *r, const poly *a, const poly *b)
{
	uint16_t vr[512];
	mul256(vr,&a->coeffs[0],&b->coeffs[0]);

	__m256i tmp0,tmp1;

	for (int i=0; i<16; i++)
	{
		tmp0 = _mm256_loadu_si256((__m256i *)&vr[i*16]);
		tmp1 = _mm256_loadu_si256((__m256i *)&vr[i*16+256]);
		tmp0 = _mm256_sub_epi16(tmp0,tmp1);

		_mm256_storeu_si256((__m256i *)&r->coeffs[i*16], tmp0);
	}
}
