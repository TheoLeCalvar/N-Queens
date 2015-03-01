#ifndef _chessboard_h_
#define _chessboard_h_

#include "types.h"

typedef struct chessboard {
        size_t          size;
        bf_dyn_t        cols; //bitset, available cols
        bf_dyn_t        rows; // bitset, available rows
        u32 *           queens; //position of the queen
} cb_t;

void    cb_init(size_t size, cb_t* cb);

//return the number of conflicting queens
int     cb_validate(const cb_t* cb);


#endif