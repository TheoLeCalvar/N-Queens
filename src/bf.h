/* -------------------------------------------------------------------------
 * bf module
 *
 * bf stands for "bits field".
 * This module offers functions to manipulate optimized bits fields.
 * bit fields are **staticaly** defined in memory with a **constant** size.
 * ------------------------------------------------------------------------- */

#ifndef _bf_h_
#define _bf_h_

#include <stdlib.h>
#include <stdint.h>

/* Maximum number of bits. */
#ifndef BF_MAX_BITS
        #define BF_MAX_BITS  256
#endif

#if (BF_MAX_BITS % 64) != 0
        #error "BF_MAX_BITS must be a multiple of 64"
#endif

/* Number of uintx_t needed to store a bits field */

__attribute__ ((aligned(16)))
typedef struct node_bf {
#ifdef __x86_64__
#define BF_SIZE (BF_MAX_BITS / 64)
        uint64_t field[BF_SIZE];
#else
#define BF_SIZE (BF_MAX_BITS / 32)
        uint32_t field[BF_SIZE];
#endif
} bf_t;


/* Don't use the same data format with all architectures */
#ifdef __x86_64__

typedef uint64_t bf_type_t;
#define BF_PART_SIZE    64
#define __ctz           __builtin_ctzl


#define bf_get(bf,n) \
        ((bf[(n) / 64] >> ((n) & 63)) & 1LL)

#define bf_set(bf,n) \
        (bf[(n) / 64] |= (1LL << ((n) & 63)))

#define bf_unset(bf,n) \
        (bf[(n) / 64] &= ~(1LL << ((n) & 63)))

#else
typedef uint32_t bf_type_t;
#define BF_PART_SIZE    32
#define __ctz           __builtin_ctz

#define bf_get(bf,n) \
        ((bf[(n) / 32] >> ((n) & 31)) & 1LL)

#define bf_set(bf,n) \
        (bf[(n) / 32] |= (1LL << ((n) & 31)))

#define bf_unset(bf,n) \
        (bf[(n) / 32] &= ~(1LL << ((n) & 31)))

#endif

/**
 * Offers to limit the number of bits on which operations will be processed.
 * @param size  The number of bits.
 */
// void bf_set_max_size(int size);

/**
 * Initialize a bits field with given value.
 * @param def   If != 0, then all the bits will be set to 1.
 */
void bf_init(bf_t* bf, int def);

/**
 * Like bf_init, but start at offset "off".
 */
void bf_init_from(bf_t* bf, size_t off);

/**
 * Make an "and" operation between two bitsets.
 * @param src   First bits set.
 * @param dst   Second bitset, and where result will be stored.
 */
void bf_and(const bf_t* src, bf_t* dst);

/**
 * Like bf_and, but store the result in a third bf_t.
 */
void bf_and_to(const bf_t* a, const bf_t* b, bf_t* dst);

/**
 * Make the diff of two bfs.
 * @param dst   First bs.
 * @param scd   Second.
 * @return      dst - scd
 */
void bf_diff(bf_t* dst, const bf_t* src);

/**
 * Make an "or" operation between two bitsets.
 * @param src   First bits set.
 * @param dst   Second bitset, and where result will be stored.
 */
void bf_or(const bf_t* src, bf_t* dst);

/**
 * Inverte all bits of a bitfield.
 */
void bf_not(bf_t* bf);

/**
 * Like bf_not but store the result in dst
 * @param bf First bitset
 * @param dst Second bf where the result is stored
 */
void bf_not_to(const bf_t* src, bf_t* dst);

/**
 * @return 0 if the given bf is empty (no bit different from 0).
 */
int bf_empty(const bf_t* bf);

/**
 * Iterate over a bitfield.
 * @param cur   From where to iterate.
 * @return < 0 if there is no more bits setted,
 *         or the index of the next bit setted.
 */
size_t bf_get_next_setted(const bf_t* bf, size_t cur);

/**
 * @return The number of setted bits.
 */
size_t bf_count(const bf_t* bf);

/**
 * @return "true" if the bfs are equals.
 */
int bf_equals(const bf_t* a, const bf_t* b);

/**
 * @return "true" if the a == (a & b).
 */
int bf_contains(const bf_t* a, const bf_t* b);

void bf_print(const bf_t* bf, size_t max);

void bf_to_str(const bf_t* bf, char* buf);

/**
 * Increment the bitfield by 1
 */
void bf_incr(bf_t* bf);

/**
 *Increment the bitfield by n
 */
void bf_incr_n(bf_t* bf, int n);

#endif

