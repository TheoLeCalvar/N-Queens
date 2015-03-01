#ifndef _bf_dyn_h_
#define _bf_dyn_h_

#include <stdlib.h>
#include <stdio.h>
#include "types.h"


typedef struct node_bf_dyn {
        size_t  size;
        size_t  nb_chunk;
#ifdef __x86_64__
        u64*    bf;
#else
        u32*    bf;
#endif
} bf_dyn_t;



#ifdef __x86_64__
#define BF_DYN_CHUNK_SIZE 64
#define __ctz           __builtin_ctzl

        typedef u64     bf_dyn_type_t;

#define bf_dyn_get(bf,n) \
        ((bf[(n) / 64] >> ((n) & 63)) & 1LL)

#define bf_dyn_set(bf,n) \
        (bf[(n) / 64] |= (1LL << ((n) & 63)))

#define bf_dyn_unset(bf,n) \
        (bf[(n) / 64] &= ~(1LL << ((n) & 63)))

#else
#define BF_DYN_CHUNK_SIZE 32
#define __ctz           __builtin_ctz

        typedef u32     bf_dyn_type_t;

#define bf_dyn_get(bf,n) \
        ((bf[(n) / 32] >> ((n) & 31)) & 1LL)

#define bf_dyn_set(bf,n) \
        (bf[(n) / 32] |= (1LL << ((n) & 31)))

#define bf_dyn_unset(bf,n) \
        (bf[(n) / 32] &= ~(1LL << ((n) & 31)))

#endif


void    bf_dyn_init(bf_dyn_t* bf, size_t size);

void    bf_dyn_init_from(bf_dyn_t* bf, size_t size, size_t from);

void    bf_dyn_clean(bf_dyn_t* bf);

void    bf_dyn_and(const bf_dyn_t* src, bf_dyn_t* dst);

void    bf_dyn_and_to(const bf_dyn_t* a, const bf_dyn_t* b, bf_dyn_t* dst);

void    bf_dyn_diff(bf_dyn_t* dst, const bf_dyn_t* src);

void    bf_dyn_or(const bf_dyn_t* src, bf_dyn_t* dst);

void    bf_dyn_not(bf_dyn_t* bf);

void    bf_dyn_not_to(const bf_dyn_t* src, bf_dyn_t* dst);

int     bf_dyn_empty(const bf_dyn_t* bf);

size_t  bf_dyn_get_next_bit(const bf_dyn_t* bf, size_t cur);

size_t  bf_dyn_count(const bf_dyn_t* bf);

int     bf_dyn_equals(const bf_dyn_t* a, const bf_dyn_t* b);

int     bf_dyn_contains(const bf_dyn_t* a, const bf_dyn_t* b);

void    bf_dyn_print(const bf_dyn_t* bf, size_t max);

void    bf_dyn_to_str(const bf_dyn_t* bf, char* buf);

void    bf_dyn_incr(bf_dyn_t* bf);

void    bf_dyn_incr_n(bf_dyn_t* bf, int n);




#endif
