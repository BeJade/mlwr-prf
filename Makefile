CC		= /usr/bin/cc
CFLAGS		= -Wall -g -O3 -fomit-frame-pointer -msse2avx -march=native -fPIC

all: speed

SOURCES = fips202.c fips202x4.c genmatrix.c keccak4x/KeccakP-1600-times4-SIMD256.o \
	prf.c poly.c

HEADERS = params.h genmatrix.h prf.h fips202x4.h poly.h polyvec.h

speed: $(SOURCES) $(HEADERS) speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCES) randombytes.c cpucycles.c speed.c -o speed

keccak4x/KeccakP-1600-times4-SIMD256.o: keccak4x/KeccakP-1600-times4-SIMD256.c \
						keccak4x/align.h \
						keccak4x/brg_endian.h \
						keccak4x/KeccakP-1600-times4-SIMD256.c \
						keccak4x/KeccakP-1600-times4-SnP.h \
						keccak4x/KeccakP-1600-unrolling.macros \
						keccak4x/SIMD256-config.h
	$(CC) $(CFLAGS) -c keccak4x/KeccakP-1600-times4-SIMD256.c -o $@

.PHONY: clean

clean:
	rm -f speed
