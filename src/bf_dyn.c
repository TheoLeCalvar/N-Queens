#include "bf_dyn.h"


void    bf_dyn_init(bf_dyn_t* bf, size_t size) {
        bf->size = size;
        bf->nb_chunk = size / BF_DYN_CHUNK_SIZE + 1;
        bf->bf = calloc(bf->nb_chunk, sizeof(bf_dyn_type_t));

        if (!bf->bf) {
                bf->size = 0;
                bf->nb_chunk = 0;
                return;
        }
}

void    bf_dyn_init_from(bf_dyn_t* bf, size_t size, size_t from) {
        bf->size = size;
        bf->nb_chunk = size / BF_DYN_CHUNK_SIZE + 1;
        bf->bf = calloc(bf->nb_chunk, sizeof(bf_dyn_type_t));

        if (!bf->bf) {
                bf->size = 0;
                bf->nb_chunk = 0;
                return;
        }

        for (size_t i = 0; i < from; ++i)
                bf_dyn_unset(bf->bf, i);

        for (size_t i = from; i < bf->size; ++i)
                bf_dyn_set(bf->bf, i);
}

void    bf_dyn_clean(bf_dyn_t* bf) {
        if (bf->size != 0) {
                bf->size = 0;
                bf->nb_chunk = 0;
                free(bf->bf);
        }
}

#ifdef __SSE2__
#include "bf_dyn_sse4.c"
#else
void    bf_dyn_and(const bf_dyn_t* src, bf_dyn_t* dst) {
        if (src->size != dst->size) return;

        for (int i = 0; i < src->nb_chunk; i++) {
                dst->bf[i] &= src->bf[i];
        }
}

void    bf_dyn_and_to(const bf_dyn_t* a, const bf_dyn_t* b, bf_dyn_t* dst) {
        if (src->size != dst->size || src->size != dst->size) return;

        for (int i = 0; i < src->nb_chunk; i++) {
                dst->bf[i] = a->bf[i] & b->bf[i];
        }
}
#endif

void    bf_dyn_diff(bf_dyn_t* dst, const bf_dyn_t* src) {
        if (src->size != dst->size) return;

        for (int i = 0; i < src->size; i++) {
                dst->bf[i] &= ~src->bf[i];
        }
}

void    bf_dyn_or(const bf_dyn_t* src, bf_dyn_t* dst) {
        if (src->size != dst->size) return;

        for (int i = 0; i < src->size; i++) {
                dst->bf[i] |= src->bf[i];
        }
}

void    bf_dyn_not(bf_dyn_t* bf) {
        for (size_t i = 0; i < bf->nb_chunk - 1; ++i)
                bf->bf[i] = ~bf->bf[i];

        for (size_t i = (bf->nb_chunk - 1) * BF_DYN_CHUNK_SIZE; i < bf->size; ++i)
                if (bf_dyn_get(bf->bf, i)) 
                        bf_dyn_unset(bf->bf, i);
                else 
                        bf_dyn_set(bf->bf, i);
}

void    bf_dyn_not_to(const bf_dyn_t* src, bf_dyn_t* dst) {
        if (src->size != dst->size) return;

        for (size_t i = 0; i < src->nb_chunk; ++i)
                dst->bf[i] = !src->bf[i];

}

int     bf_dyn_empty(const bf_dyn_t* bf) {
        if (bf->size == 0) return 1;

        for (size_t i = 0; i < bf->nb_chunk - 1; ++i)
                if (bf->bf[i] != 0) return 0;


        if (bf->bf[bf->nb_chunk - 1] & (~((bf_dyn_type_t)-1 << bf->size % BF_DYN_CHUNK_SIZE))) return 0;



        return 1;
}

size_t  bf_dyn_get_next_bit(const bf_dyn_t* bf, size_t cur) {
        if (bf->size == 0) return -1;

        size_t i = 0;
        if (cur == -1) {
                goto simple;
        }
        
        i = cur / BF_DYN_CHUNK_SIZE;

        // First, will need to mask the (cur - i * 64) first bits of the
        // first bf part.
        bf_dyn_type_t mask = ((bf_dyn_type_t) -1) << (cur - i * BF_DYN_CHUNK_SIZE + 1);
        bf_dyn_type_t fbfp = bf->bf[i] & mask;

        if (fbfp != 0) {
                cur = i * BF_DYN_CHUNK_SIZE;
                if (cur < bf->size) {
                        cur += (size_t) __ctz(fbfp);
                        return (cur < bf->size) ? (cur) : (-1);
                }
        } else {
                i++;
simple:
                while (bf->bf[i] == 0 && i < bf->nb_chunk) {
                        i++;
                }
                cur = i * BF_DYN_CHUNK_SIZE;
                if (cur < bf->size) {
                        cur += (size_t) __ctz(bf->bf[i]);
                        return (cur < bf->size) ? (cur) : (-1);
                }
        }

        return (size_t) -1;
}
size_t  bf_dyn_count(const bf_dyn_t* bf) {
        size_t count = 0;
        int i = 0;
        while (i < bf->nb_chunk && bf->bf[i] == 0) {
                i++;
        }
#ifdef __x86_64__
        while (i < bf->nb_chunk) {
                count += __builtin_popcountl(bf->bf[i]);
                i++;
        }
#else
        while (i < bf->nb_chunk) {
                count += __builtin_popcount(bf->bf[i]);
                i++;
        }
#endif
        return count;
}

int     bf_dyn_equals(const bf_dyn_t* a, const bf_dyn_t* b) {
        if (a->size != b->size) return 0;

        for (size_t i = 0; i < a->nb_chunk; ++i)
                if (a->bf[i] != b->bf[i]) return 0;

        return 1;
}

int     bf_dyn_contains(const bf_dyn_t* a, const bf_dyn_t* b) {
        fprintf(stderr, "Not yet implemented\n");
        return 0;
}

void    bf_dyn_print(const bf_dyn_t* bf, size_t max) {
        for (size_t i = 0; i < max - 1; ++i)
                if (bf_dyn_get(bf->bf, i))
                        printf("%zu ", i);

        if (bf_dyn_get(bf->bf, max - 1))
                printf("%zu\n", max - 1);
        else
                printf("\n");

}

void    bf_dyn_to_str(const bf_dyn_t* bf, char* buf) {
        fprintf(stderr, "Not implement yet\n");
}

void    bf_dyn_incr(bf_dyn_t* bf) {
        char carry;
        int i = 0;

        do {
                carry = (bf->bf[i] == -1);
                bf->bf[i]++;
                ++i;
        } while (carry && (i < bf->nb_chunk));

        bf_dyn_unset(bf->bf, bf->size);
}

void    bf_dyn_incr_n(bf_dyn_t* bf, int n) {
        char carry;
        int i = 1;

        carry = (bf->bf[0] + n < bf->bf[0]);
        bf->bf[0] += n;
        

        while (carry && (i < bf->nb_chunk)) {
                carry = (bf->bf[i] == -1);
                bf->bf[i]++;
                ++i;
        }

        bf_dyn_unset(bf->bf, bf->size);
}


#ifdef TEST

#include "test.h"

int main(int argc, char** argv) {
        bf_dyn_t a, b;
        bf_dyn_init(&a, 315);
        bf_dyn_init(&b, 315);
        bf_dyn_not(&b);

        TEST_ASSERT("Check bf_dyn_empty", bf_dyn_empty(&a));
        TEST_ASSERT("Check bf_dyn_count", bf_dyn_count(&b) == 315);
        TEST_ASSERT("Check bf_dyn_equals", !bf_dyn_equals(&a, &b));

        bf_dyn_set(a.bf, 15);
        TEST_ASSERT("Check bf_dyn_set & bf_dyn_get", bf_dyn_get(a.bf, 15));

        bf_dyn_and(&a, &b);
        TEST_ASSERT("Check bf_dyn_and", bf_dyn_count(&b) == 1);

        bf_dyn_clean(&b);
        bf_dyn_init(&b, 315);
        bf_dyn_set(b.bf, 0);
        bf_dyn_set(b.bf, 15);
        bf_dyn_set(b.bf, 47);
        bf_dyn_set(b.bf, 65);
        bf_dyn_set(b.bf, 76);
        bf_dyn_set(b.bf, 313);
        bf_dyn_set(b.bf, 318);
        size_t it = -1;
        TEST_ASSERT("Check bf_dyn_get_next_bit (1)",
                    (it = bf_dyn_get_next_bit(&b, it)) == 0);
        TEST_ASSERT("Check bf_dyn_get_next_bit (2)",
                    (it = bf_dyn_get_next_bit(&b, it)) == 15);
        TEST_ASSERT("Check bf_dyn_get_next_bit (3)",
                    (it = bf_dyn_get_next_bit(&b, it)) == 47);
        TEST_ASSERT("Check bf_dyn_get_next_bit (4)",
                    (it = bf_dyn_get_next_bit(&b, it)) == 65);
        TEST_ASSERT("Check bf_dyn_get_next_bit (5)",
                    (it = bf_dyn_get_next_bit(&b, it)) == 76);
        TEST_ASSERT("Check bf_dyn_get_next_bit (6)",
                    (it = bf_dyn_get_next_bit(&b, it)) == 313);
        TEST_ASSERT("Check bf_dyn_get_next_bit (7)",
                    (it = bf_dyn_get_next_bit(&b, it)) == -1);

        bf_dyn_clean(&a);
        bf_dyn_init(&a, 512);
        bf_dyn_diff(&a, &b);
        TEST_ASSERT("Check bf_dyn_diff", !bf_dyn_get(a.bf, 47));

        bf_dyn_clean(&a);
        bf_dyn_init(&a, 512);
        a.bf[0] = 1087;
        bf_dyn_incr(&a);
        TEST_ASSERT("Check bf_dyn_incr no carry",
                    (a.bf[0] == 1088));

        bf_dyn_clean(&a);
        bf_dyn_init(&a, 512);
        a.bf[0] = -1;
        bf_dyn_incr(&a);
        TEST_ASSERT("Check bf_dyn_incr with carry",
                    ((a.bf[0] == 0) && (a.bf[1] == 1)));
        bf_dyn_clean(&a);
        bf_dyn_init(&a, 512);
        a.bf[0] = 1087;
        bf_dyn_incr_n(&a, 16);
        TEST_ASSERT("Check bf_dyn_incr_n without carry",
                    (a.bf[0] == 1103) && (a.bf[1] == 0));
        bf_dyn_clean(&a);
        bf_dyn_init(&a, 512);
        a.bf[0] = -1;
        bf_dyn_incr_n(&a, 8);
        TEST_ASSERT("Check bf_dyn_inc_n whith carry",
                    ((a.bf[0] == 7) && (a.bf[1] == 1)));

        bf_dyn_clean(&a);
        bf_dyn_init(&a, 375);
        bf_dyn_not(&a);
        bf_dyn_clean(&b);
        bf_dyn_init(&b, 375);
        bf_dyn_not(&b);
        TEST_ASSERT("Check bf_dyn_not", (bf_dyn_equals(&a, &b)));

        bf_dyn_not_to(&a, &b);
        TEST_ASSERT("Check bf_dyn_not_to", (bf_dyn_empty(&b)));

        return 0;
}

#endif