#include "bf.h"
#include <emmintrin.h>

#ifdef __x86_64__
#define inc_val         2
#else
#define inc_val         4
#endif

void    bf_dyn_and(const bf_dyn_t* src, bf_dyn_t* dst) {
        if (src->size != dst->size) return;

        __m128i* r = (__m128i*) &dst->bf[0];
        for (int i = 0; i < src->nb_chunk; i += inc_val) {
                __m128i* a = (__m128i*) &src->bf[i];
                __m128i* b = (__m128i*) &dst->bf[i];
                *r  = _mm_and_si128(*a, *b);
                r++;
        }
}

void    bf_dyn_and_to(const bf_dyn_t* a, const bf_dyn_t* b, bf_dyn_t* dst) {
        if (a->size != b->size || a->size != dst->size) return;

        __m128i* r = (__m128i*) &dst->bf[0];
        __m128i* pa = (__m128i*) a->bf;
        __m128i* pb = (__m128i*) b->bf;
        for (int i = 0; i < a->nb_chunk; i += inc_val) {
                *r  = _mm_and_si128(*pa, *pb);
                pa++;
                pb++;
                r++;
        }
}

