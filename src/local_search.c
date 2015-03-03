#include "local_search.h"

extern void init_cb(cb_t* cb) {
        for (size_t i = 0; i < cb->size; ++i) {
                cb->queens[i] = cb_best_row(cb, i);
        }

        cb_print(cb);
}



int     local_search(cb_t* cb) {
        size_t  try = 0;
        u32 *   buf_col;
        u32 *   buf_row;

        init_cb(cb);
        buf_col = malloc(cb->size * sizeof(u32));
        buf_row = malloc(cb->size * sizeof(u32));

        if (!buf_col || !buf_row) {
                log_err("Can't allocate buffer in %s:%d", __FILE__, __LINE__);
                return 1;
        }

        if (cb->size == -1) {
                log_err("Can't allocate chessboard in %s:%d", __FILE__, __LINE__);
                return 1;
        }

        while (cb_conflicts(cb, buf_col)) {

        }

        printf("Found a solution in %zu switchs.\n", try);

        return 0;
}

#ifdef TEST

#include "test.h"

int main(int argc, char **argv) {

}

#endif