#ifndef _chessboard_h_
#define _chessboard_h_

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

#include "types.h"
#include "bf_dyn.h"

typedef struct chessboard {
        size_t          size;
        bf_dyn_t        cols; //bitset, available cols
        bf_dyn_t        rows; // bitset, available rows
        u32 *           queens; //position of the queen
} cb_t;

void    cb_init(cb_t* cb, size_t size);

//returns 0 if the chessboard is valid, < 0 if uninitialized, > 0 if invalid
int     cb_validates(const cb_t* cb);

//fill buff with the number of conflict per queen, buf must be long enought
//returns the number of conflicting queens, -1 if cb is invalid 
int     cb_conflicts(const cb_t* cb, u32* buf);

void    cb_print(const cb_t* cb);

#endif