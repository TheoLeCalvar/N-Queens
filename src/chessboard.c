#include "chessboard.h"

void    cb_init(cb_t* cb, size_t size) {
        cb->size = size;

        bf_dyn_init(&cb->cols, size);
        bf_dyn_init(&cb->rows, size);
        cb->queens = malloc(size * sizeof(u32));

        if (!cb->cols.size || !cb->rows.size || !cb->queens) {
                cb->size = 0;
                bf_dyn_clean(&cb->cols);
                bf_dyn_clean(&cb->rows);
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

        for (size_t i = 0; i < cb->size; ++i)
                if (cb->queens[i] != -1)
                        for (size_t j = i+1; j < cb->size; ++j)
                                if (cb->queens[i] != -1 && 
                                    abs(i - j) == abs(cb->queens[i] - cb->queens[j])
                                ) { 
                                        return 1;
                                }

        return 0;
}

int     cb_conflicts(const cb_t* cb, u32* buf) {
        if (!cb->size) {
                log_err("Call cb_conflicts on non valid cb at %s:%d", __FILE__, __LINE__);
                return -1;
        }

        u32     conflict = 0;

        for (size_t i = 0; i < cb->size; ++i)
                buf[i] = 0;

        for (size_t i = 0; i < cb->size; ++i)
                if (cb->queens[i] != -1)
                        for (size_t j = i+1; j < cb->size; ++j)
                                if (cb->queens[j] != -1 &&
                                    abs(i - j) == abs(cb->queens[i] - cb->queens[j])
                                ) {
                                        ++buf[i];
                                        ++buf[j];
                                        ++conflict;
                                        log_info("Found conflicting queens (%lu,%d), (%lu, %d)",
                                                i, cb->queens[i], j, cb->queens[j]);
                                }


        return conflict;
}

void    cb_print(const cb_t* cb) {
        if (!cb->size) printf("Uninitialized chessboard\n");

        printf("  ");
        for(size_t i = 0; i < cb->size; ++i)
                printf("%zu ", i);

        printf("\n");

        for (size_t i = 0; i < cb->size; ++i) {
                printf("%zu ", i);
                for (size_t j = 0; j < cb->size; ++j) {
                        printf("%c ", (cb->queens[j] == i) ? '#' : '.');
                }
                printf("\n");
        }

        printf("Todo: format number to prevent bad display\n");

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

        cb_print(&cb);

        return 0;
}

#endif