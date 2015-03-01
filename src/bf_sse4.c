#include "bf.h"
#include <emmintrin.h>

#ifdef __x86_64__
#define inc_val         2
#else
#define inc_val         4
#endif

void bf_and(const bf_t* src, bf_t* dst) {
        __m128i* r = (__m128i*) &dst->field[0];
        for (int i = 0; i < BF_SIZE; i += inc_val) {
                __m128i* a = (__m128i*) &src->field[i];
                __m128i* b = (__m128i*) &dst->field[i];
                *r  = _mm_and_si128(*a, *b);
                r++;
        }
}

void bf_and_to(const bf_t* a, const bf_t* b, bf_t* dst) {
        __m128i* r = (__m128i*) &dst->field[0];
        __m128i* pa = (__m128i*) a->field;
        __m128i* pb = (__m128i*) b->field;
        for (int i = 0; i < BF_SIZE; i += inc_val) {
                *r  = _mm_and_si128(*pa, *pb);
                pa++;
                pb++;
                r++;
        }
}

