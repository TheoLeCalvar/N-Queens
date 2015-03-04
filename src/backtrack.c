#include "backtrack.h"


extern int bt_rec(cb_t* cb, size_t col) {
        //weare not at the end, now we have to select where to place the colth queens
        if (col < cb->size) {
                size_t queen_place = -1;

                while ((queen_place = bf_dyn_get_next_bit(&(cb->rows), queen_place)) != -1) {
                        //we place the queen in the first available slot
                        cb->queens[col] = queen_place;
                        bf_dyn_unset(cb->rows.bf, queen_place);

                        //if the temp construction is valid we propagate it
                        //and if we found a solution, propagate the return
                        if (!cb_validates_fast(cb, queen_place, col) && !bt_rec(cb, col + 1))
                                return 0;

                        //else we have to undo what we have done and try the next place
                        bf_dyn_set(cb->rows.bf, queen_place);
                        cb->queens[col] = -1;

                }

                return 1;

        }
        //we are at the bottom, only one choice, evaluate the leaf
        else
                return cb_validates(cb);
}


int backtrack(cb_t* cb) {
        return bt_rec(cb, 0);
}