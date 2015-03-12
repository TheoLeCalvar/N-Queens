#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "bf.h"

void bf_init(bf_t* bf, int x) {
        for (int i = 0; i < BF_SIZE; i++) {
                bf->field[i] = (x) ? (bf_type_t) -1 : 0;
        }
}

void bf_init_from(bf_t* bf, size_t off) {
        for (int i = 0; i < off; i++) {
                bf_unset(bf->field, i);       
        }
        for (int i = off; i < BF_MAX_BITS; i++) {
                bf_set(bf->field, i);
        }
}

#ifdef __SSE2__
#include "bf_sse4.c"
#else
void bf_and(const bf_t* src, bf_t* dst) {
        for (int i = 0; i < BF_SIZE; i++) {
                dst->field[i] &= src->field[i];
        }
}

void bf_and_to(const bf_t* a, const bf_t* b, bf_t* dst) {
        for (int i = 0; i < BF_SIZE; i++) {
                dst->field[i] = a->field[i] & b->field[i];
        }
}
#endif

void bf_diff(bf_t* dst, const bf_t* src) {
        for (int i = 0; i < BF_SIZE; i++) {
                dst->field[i] &= ~src->field[i];
        }
}

void bf_or(const bf_t* src, bf_t* dst) {
        for (int i = 0; i < BF_SIZE; i++) {
                dst->field[i] |= src->field[i];
        }
}

void bf_not(bf_t* bf) {
        for (int i = 0; i < BF_SIZE; ++i) {
                bf->field[i] = ~bf->field[i];
        }
}

void bf_not_to(const bf_t* bf, bf_t* dst) {
        for (int i = 0; i < BF_SIZE; ++i) {
                dst->field[i] = ~bf->field[i];
        }
}

int bf_empty(const bf_t* bf) {
        for (size_t i = 0; i < BF_SIZE; i++) {
                if (bf->field[i] != 0) {
                        return 0;
                }
        }
        return 1;
}

size_t bf_get_next_setted(const bf_t* bf, size_t cur) {
        size_t i = 0;
        if (cur == -1) {
                goto simple;
        }
        
        i = cur / BF_PART_SIZE;

        if (cur % BF_PART_SIZE == BF_PART_SIZE - 1) {
                ++i;
                goto simple;
        }

        // First, will need to mask the (cur - i * 64) first bits of the
        // first bf part.
        bf_type_t mask = ((bf_type_t) -1) << (cur - i * BF_PART_SIZE + 1);
        bf_type_t fbfp = bf->field[i] & mask;

        if (fbfp != 0) {
                cur = i * BF_PART_SIZE;
                if (cur < BF_MAX_BITS) {
                        cur += (size_t) __ctz(fbfp);
                        return cur;
                }
        } else {
                i++;
simple:
                while (bf->field[i] == 0 && i < BF_SIZE) {
                        i++;
                }
                cur = i * BF_PART_SIZE;
                if (cur < BF_MAX_BITS) {
                        cur += (size_t) __ctz(bf->field[i]);
                        return cur;
                }
        }

        return (size_t) -1;
}

size_t bf_count(const bf_t* bf) {
        size_t count = 0;
        int i = 0;
        while (i < BF_SIZE && bf->field[i] == 0) {
                i++;
        }
#ifdef __x86_64__
        while (i < BF_SIZE) {
                count += __builtin_popcountl(bf->field[i]);
                i++;
        }
#else
        while (i < BF_SIZE) {
                count += __builtin_popcount(bf->field[i]);
                i++;
        }
#endif
        return count;
}

int bf_equals(const bf_t* a, const bf_t* b) {
        for (int i = 0; i < BF_SIZE; i++) {
                if (a->field[i] != b->field[i]) {
                        return 0;
                }
        }
        return 1;
}

int bf_contains(const bf_t* a, const bf_t* b) {
        bf_t inter;

        bf_and_to(a, b, &inter);
        return bf_equals(a, &inter);
}

void bf_print(const bf_t* bf, size_t max) {
        for (size_t i = 0; i < max - 1; i++) {
                if (bf_get(bf->field, i)) {
                        printf("%zu ", i);
                }
        }
        if (bf_get(bf->field, (max - 1))) {
                printf("%zu\n", max - 1);
        }
        else {
                printf("\n");
        }
}

void bf_to_str(const bf_t* bf, char* buf) {
        size_t idx = -1;
        char* p = buf;
        *p = '\0';
        do {
                idx = bf_get_next_setted(bf, idx);
                if (idx == -1) {
                        break;
                }
                p += sprintf(p, "%zu ", idx);
        } while (1);
}

void bf_incr(bf_t* bf) {
        char carry;
        int i = 0;

        do {
                carry = (bf->field[i] == -1);
                bf->field[i]++;
                ++i;
        } while (carry && (i < BF_SIZE));
}

void bf_incr_n(bf_t* bf, int n) {
        char carry;
        int i = 1;

        carry = (bf->field[0] + n < bf->field[0]);
        bf->field[0] += n;
        

        while (carry && (i < BF_SIZE)) {
                carry = (bf->field[i] == -1);
                bf->field[i]++;
                ++i;
        }
}

#ifdef TEST

#include "test.h"

int main(int argc, char** argv) {
        bf_t a, b;
        bf_init(&a, 0);
        bf_init(&b, 1);

        TEST_ASSERT("Check bf_empty", bf_empty(&a));
        TEST_ASSERT("Check bf_count", bf_count(&b) == BF_MAX_BITS);
        TEST_ASSERT("Check bf_equals", !bf_equals(&a, &b));

        bf_set(a.field, 15);
        TEST_ASSERT("Check bf_set & bf_get", bf_get(a.field, 15));

        bf_and(&a, &b);
        TEST_ASSERT("Check bf_and", bf_count(&b) == 1);

        bf_set(b.field, 0);
        bf_set(b.field, 46);
        bf_set(b.field, 63);
        bf_set(b.field, 64);
        bf_set(b.field, 65);
        bf_set(b.field, 76);
        bf_set(b.field, 87);
        bf_set(b.field, 107);
        size_t it = -1;
        TEST_ASSERT("Check bf_get_next_setted (1)",
                    (it = bf_get_next_setted(&b, it)) == 0);
        TEST_ASSERT("Check bf_get_next_setted (2)",
                    (it = bf_get_next_setted(&b, it)) == 15);
        TEST_ASSERT("Check bf_get_next_setted (3)",
                    (it = bf_get_next_setted(&b, it)) == 46);
        TEST_ASSERT("Check bf_get_next_setted (4)",
                    (it = bf_get_next_setted(&b, it)) == 63);        
        TEST_ASSERT("Check bf_get_next_setted (5)",
                    (it = bf_get_next_setted(&b, it)) == 64);
        TEST_ASSERT("Check bf_get_next_setted (6)",
                    (it = bf_get_next_setted(&b, it)) == 65);
        TEST_ASSERT("Check bf_get_next_setted (7)",
                    (it = bf_get_next_setted(&b, it)) == 76);
        TEST_ASSERT("Check bf_get_next_setted (8)",
                    (it = bf_get_next_setted(&b, it)) == 87);
        TEST_ASSERT("Check bf_get_next_setted (9)",
                    (it = bf_get_next_setted(&b, it)) == 107);
        TEST_ASSERT("Check bf_get_next_setted (10)",
                    (it = bf_get_next_setted(&b, it)) == -1);

        bf_init(&a, 1);
        bf_diff(&a, &b);

        TEST_ASSERT("Check bf_diff", !bf_get(a.field, 46));

        bf_init(&a, 0);
        a.field[0] = 1087;
        bf_incr(&a);
        TEST_ASSERT("Check bf_incr no carry",
                    (a.field[0] == 1088));

        bf_init(&a, 0);
        a.field[0] = -1;
        bf_incr(&a);
        TEST_ASSERT("Check bf_incr with carry",
                    ((a.field[0] == 0) && (a.field[1] == 1)));

        bf_init(&a, 0);
        a.field[0] = 1087;
        bf_incr_n(&a, 16);
        TEST_ASSERT("Check bf_incr_n without carry",
                    (a.field[0] == 1103) && (a.field[1] == 0));

        bf_init(&a, 0);
        a.field[0] = -1;
        bf_incr_n(&a, 8);
        TEST_ASSERT("Check bf_inc_n whith carry",
                    ((a.field[0] == 7) && (a.field[1] == 1)));


        bf_init(&a, 0);
        bf_init(&b, 1);
        bf_not(&a);
        TEST_ASSERT("Check bf_not", (bf_equals(&a, &b)));

        bf_not_to(&a, &b);
        TEST_ASSERT("Check bf_not_to", (bf_empty(&b)));

          return 0;
}

#endif

