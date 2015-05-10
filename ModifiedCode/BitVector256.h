//TomAndJerry BitVector256.h
#include <stdint.h> // uint16_t

static const unsigned VECTOR_SIZE = 16;

typedef struct BitVector256 BitVector256;
struct BitVector256{
    uint16_t vector[(VECTOR_SIZE * VECTOR_SIZE) / (8*sizeof(uint16_t))],
};

void set(BitVector256* bv, unsigned x, unsigned y);
void clear(BitVector256* bv, unsigned x, unsigned y);
bool get(BitVector256* bv, unsigned x, unsigned y);
void clearAll(BitVector256* bv);
void setAll(BitVector256* bv);


