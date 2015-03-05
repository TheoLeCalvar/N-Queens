#include "backtrack.h"

//amélioration possible, pour la ligne, réduire les valeurs possible en supprimant les
//valeur dans les diagonales des reines précédentes
extern int bt_rec(cb_t* cb, size_t col) {
        //weare not at the end, now we have to select where to place the colth queens
        if (col < cb->size) {
                bf_t    available;
                size_t  queen_place = -1;

                //at start all rows are availables
                bf_init(&available, 1);

                //we remove cells in diagonales of the previous queens
                //< MAX_QUEENS in both cases because we are using unsigned
                for (size_t i = 0; i < col; ++i) {
                        if (cb->queens[i] + (col - i) < MAX_QUEENS) bf_unset(available.field, cb->queens[i] + (col - i));
                        if (cb->queens[i] - (col - i) < MAX_QUEENS) bf_unset(available.field, cb->queens[i] - (col - i));
                }

                //merge the availables rows from the diagonals and the chessboards rows
                bf_and(&cb->rows, &available);


                while ((queen_place = bf_get_next_setted(&available, queen_place)) < cb->size) {
                        //we place the queen in the first available slot
                        cb->queens[col] = queen_place;
                        bf_unset(cb->rows.field, queen_place);

                        // cb_print(cb);

                        //if the temp construction is valid we propagate it
                        //and if we found a solution, propagate the return
                        if (!bt_rec(cb, col + 1))
                                return 0;

                        //else we have to undo what we have done and try the next place
                        bf_set(cb->rows.field, queen_place);
                        cb->queens[col] = -1;

                }

                return 1;

        }
        //we are at the bottom, only one choice, evaluate the leaf
        else
                return cb_validates(cb);
}


int backtrack(cb_t* cb) {
        // cl_init();

        return bt_rec(cb, 0);
}