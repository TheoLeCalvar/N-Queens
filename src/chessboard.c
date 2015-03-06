#include "chessboard.h"


void    cb_init(cb_t* cb, size_t size) {
        if (size > MAX_QUEENS) {
                log_err("Can't initialize chessboard, size > %d.\nAdjust MAX_QUEENS in chessboard.h.", MAX_QUEENS);
                cb->size = 0;
                return;
        }
        cb->size = size;

        bf_init(&cb->cols, 1);
        bf_init(&cb->rows, 1);


        if (0) {
                cb->size = 0;
                log_err("Can't initialize chessboard.");
                return;
        }

        for (size_t i = 0; i < cb->size; ++i)      
                cb->queens[i] = -1;

}       

int     cb_validates(const cb_t* cb) {
        if (!cb->size) {
                log_info("Call cb_validates on unintialized chessboard");
                return -1;
        }

        for (size_t i = 0; i < cb->size; ++i) {
                for (size_t j = i+1; j < cb->size; ++j)
                        if (cb->queens[j] != -1 && 
                            abs(i - j) == 
                            abs(cb->queens[i] - cb->queens[j])
                        ) 
                                return 1;
        }
                        

        return 0;
}

int     cb_validates_full(const cb_t* cb) {
        if (!cb->size) return -1;

        for (size_t i = 0; i < cb->size; ++i) {
                //si c'est > size (inclus == -1)
                if (cb->queens[i] >= cb->size) return 1;
                for (size_t j = i + 1; j < cb->size; ++j)
                        if (cb->queens[i] == cb->queens[j] ||
                            abs(cb->queens[i] - cb->queens[j]) == abs(i - j))
                                return 1;
        }

        return 0;
}

int     cb_validates_fast(const cb_t* cb, size_t row, size_t col) {
        if (!cb->size) {
                log_info("Call cv_validates_fast on unintialized chessboard");
                return -1;
        }

        u32     queen;

        for (size_t i = 0; (i < cb->size) && ((queen = cb->queens[i]) != -1); ++i) {
                if (    i != col &&
                        abs(i - col) == abs(queen - row)
                   )
                        return 1;
        }

        return 0;
}

int     cb_conflicts(const cb_t* cb, u32* buf) {
        if (!cb->size) {
                log_err("Call cb_conflicts on non valid cb");
                return -1;
        }

        u32     conflict = 0;

        if (buf)
                for (size_t i = 0; i < cb->size; ++i)
                        buf[i] = 0;

        for (size_t i = 0; i < cb->size; ++i)
                if (cb->queens[i] != -1)
                        for (size_t j = i+1; j < cb->size; ++j)
                                if (cb->queens[j] != -1 &&
                                    (cb->queens[i] == cb->queens[j] ||
                                    abs(i - j) == abs(cb->queens[i] - cb->queens[j]))
                                ) {
                                        if (buf) {
                                                ++buf[i];
                                                ++buf[j];
                                        }
                                        ++conflict;
                                        log_info("Found conflicting queens (%u,%u), (%u, %u)",
                                                i, cb->queens[i], j, cb->queens[j]);
                                }


        return conflict;
}

size_t  cb_best_row(cb_t* cb, size_t col) {
        if (!cb->size) {
                log_info("Call cb_best_row on unintialized chessboard");
                return -1;
        }

        u32     orig = cb->queens[col];
        size_t  best_conflicts = -1;
        size_t  best_position = -1;

        for (size_t i = 0; i < cb->size; ++i) {
                cb->queens[col] = i;
                size_t conflicts = cb_conflicts(cb, NULL);

                if (conflicts == 0) {
                        cb->queens[col] = orig;
                        return i;                       
                }

                if (conflicts < best_conflicts) {
                        best_conflicts = conflicts;
                        best_position = i;
                }

        }

        cb->queens[col] = orig;

        return best_position;
}

void    cb_rows(cb_t* cb, size_t col, u32* buf) {
        if (!cb->size) {
                log_info("Call cb_rows on an unintialized chessboard");
                return;
        }

        u32 orig = cb->queens[col];

        for (size_t i = 0; i < cb->size; ++i) {
                cb->queens[col] = i;

                buf[i] = cb_conflicts(cb, NULL); 
        }

        cb->queens[col] = orig;
}

void    cb_display(const cb_t* cb) {
        if (!cb->size) printf("Uninitialized chessboard\n");

        printf("  x ");
        for(size_t i = 0; i < cb->size; ++i)
                printf("%3zu ", i);

        printf("\n");

        for (size_t i = 0; i < cb->size; ++i) {
                printf("%3zu ", i);
                for (size_t j = 0; j < cb->size; ++j) {
                        printf("%3c ", (cb->queens[j] == i) ? '#' : '.');
                }
                printf("\n");
        }

        printf("Todo: format number to prevent bad display\n");

}

void    cb_print(const cb_t* cb) {
        if (!cb->size) printf("unintialized chessboard\n");


        for (size_t i = 0; i < cb->size - 1; ++i)
                printf("%d, ", cb->queens[i]);

        printf("%d\n", cb->queens[cb->size - 1]);
}


#ifdef TEST

#include "test.h"

int main(int argc, char** argv) {
        cb_t    cb;
        u32     buf[8];

        cb_init(&cb, 8);

        cb.queens[0] = 0;
        cb.queens[1] = 6;
        cb.queens[2] = 3;
        cb.queens[3] = 5;
        cb.queens[4] = 7;
        cb.queens[5] = 1;
        cb.queens[6] = 4;
        cb.queens[7] = 2;

        TEST_ASSERT("Check cb_init", cb.size == 8);
        TEST_ASSERT("Check cb_validate (1)", cb_validates(&cb) == 0);

        cb.queens[0] = 4;
        cb.queens[6] = 0;

        TEST_ASSERT("Check cb_validate (2)", cb_validates(&cb) > 0);

        cb_conflicts(&cb, buf);
        TEST_ASSERT("Check cb_conflicts", 
                (       buf[0] == 0 && 
                        buf[1] == 0 &&
                        buf[2] == 0 &&
                        buf[3] == 0 &&
                        buf[4] == 0 &&
                        buf[5] == 1 && 
                        buf[6] == 1 &&
                        buf[7] == 0
                ));


        TEST_ASSERT("Check cb_best_row", cb_best_row(&cb, 5) == 1);

        cb_rows(&cb, 5, buf);
        TEST_ASSERT("Check cb_rows", 
                (
                        buf[0] == 3 &&
                        buf[1] == 1 &&
                        buf[2] == 2 && 
                        buf[3] == 2 &&
                        buf[4] == 2 &&
                        buf[5] == 1 &&
                        buf[6] == 3 &&
                        buf[7] == 2
                ));

        cb_print(&cb);

        return 0;
}

#endif