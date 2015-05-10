#include "BitVector256.h"
#include <stdlib.h>
#include <cstring> // memset

void set(BitVector256* bv, unsigned x, unsigned y){
	if(x < VECTOR_SIZE && y < VECTOR_SIZE)
		bv->vector[x] |= 1<<y;
}

void clear(BitVector256* bv, unsigned x, unsigned y){
	if(x < VECTOR_SIZE && y < VECTOR_SIZE)
		bv->vector[x] &= ~(1<<y);
}

bool get(BitVector256* bv, unsigned x, unsigned y){
	if(x < VECTOR_SIZE && y < VECTOR_SIZE)
        return (bv->vector[x] & 1<<y) != 0;
    return 0;
}

void clearAll(BitVector256* bv){
	memset(bv->vector, 0, sizeof(bv->vector));
}

void setAll(BitVector256* bv){
	memset(bv->vector, ~0, sizeof(bv->vector));
}

