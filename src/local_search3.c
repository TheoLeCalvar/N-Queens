#include "local_search3.h"
#include <sys/time.h>

#define MAX_CONFLICTS           100
#define MAX_CONFLICTS_SIZE      512

#define is_threatened(__i, __j) \
                (diagu[(u32)(__i) + (u32)(__j)] || diagd[(u32)(__i) - (u32)(__j) + (u32)cb->size])

#define is_attacked(__i, __j) \
                (diagu[(u32)(__i) + (u32)(__j)] > 1 || diagd[(u32)(__i) - (u32)(__j) + (u32)cb->size] > 1)

#define place_queen(__i, __j)                                                                   \
        ({                                                                                      \
                cb->queens[(__i)] = (__j);                                                      \
                if (++diagu[(u32)(__i) + (u32)(__j)] > 1)                                       \
                        bf_dyn_set(conflict_diagup, (u32)(__i) + (u32)(__j));                   \
                if (++diagd[(u32)(__i) - (u32)(__j) + (u32)cb->size] > 1)                       \
                        bf_dyn_set(conflict_diagdp, (u32)(__i) - (u32)(__j) + (u32)cb->size);   \
        })


#define remove_queen(__i, __j)                                                                  \
        ({                                                                                      \
                if (--diagu[(u32)(__i) + (u32)(__j)] <= 1)                                      \
                        bf_dyn_unset(conflict_diagup, (u32)(__i) + (u32)(__j));                 \
                if (--diagd[(u32)(__i) - (u32)(__j) + (u32)cb->size] <= 1)                      \
                        bf_dyn_unset(conflict_diagdp, (u32)(__i) - (u32)(__j) + (u32)cb->size); \
        })

#define swap(__ci, __cj, __qi, __qj)             \
        ({                                       \
                remove_queen((__ci), (__qi));    \
                remove_queen((__cj), (__qj));    \
                place_queen((__ci), (__qj));     \
                place_queen((__cj), (__qi));     \
        })

int     local_search3(cb_t * cb) {
        u32 *   diagu = NULL;
        u32 *   diagd = NULL;

        size_t  free_cols_size = cb->size;
        u32 *   free_cols;

        size_t  conflict_queens_size = 0;
        u32     conflict_queens[MAX_CONFLICTS_SIZE];

        bf_dyn_t  conflict_diagu;
        bf_dyn_t  conflict_diagd;
        bf_dyn_t* conflict_diagup = &conflict_diagu;
        bf_dyn_t* conflict_diagdp = &conflict_diagd;

        size_t  conflict_free = 0;
        char    conflict = 0;
        int     n = 0;

        struct timeval before_init, before_solve, end;
        size_t  init_conflicts = 0;

        diagu = calloc(2 * cb->size, sizeof(u32));
        diagd = calloc(2 * cb->size, sizeof(u32));
        free_cols = malloc(cb->size * sizeof(u32));

        bf_dyn_init(&conflict_diagu, 2 * cb->size);
        bf_dyn_init(&conflict_diagd, 2 * cb->size);

        if (!diagu || !diagd || !free_cols || !conflict_diagu.size || !conflict_diagd.size || !conflict_queens) {
                log_err("Can't initiate requiered arrays.");
                return 1;
        }


        for (size_t i = 0; i < cb->size; ++i)
                free_cols[i] = i;

        gettimeofday(&before_init, NULL);

        //initialize chessboard, we want at most MAX_CONFLICTS conflicting queens
        for (size_t i = 0; i < cb->size; ++i) {
                do {
                        size_t random = rand() % free_cols_size;

                        conflict = is_threatened(i, free_cols[random]);
                        if (conflict_free < cb->size - MAX_CONFLICTS) {
                                if (!conflict) {
                                        place_queen(i, free_cols[random]);
                                        free_cols[random] = free_cols[--free_cols_size];

                                        ++conflict_free;

                                }
                        } else {
                                place_queen(i, free_cols[random]);
                                free_cols[random] = free_cols[--free_cols_size];
                                conflict = 0;

                                for (size_t j = 0; j < i; ++j) {
                                        if (abs(i - j) == abs(cb->queens[i] - cb->queens[j])) {
                                                conflict_queens[conflict_queens_size++] = i;
                                                conflict_queens[conflict_queens_size++] = j;
                                        }
                                }
                        }

                } while (conflict);
        }

        gettimeofday(&before_solve, NULL);
        init_conflicts = conflict_diagu.nb_set + conflict_diagd.nb_set;

        //resolution
        do {
                n = 0;
                for (size_t i = 0; i < conflict_queens_size; ++i) {
                        for (size_t j = 0; j < conflict_queens_size; ++j) {
                                u32 ci = conflict_queens[i];
                                u32 qi = cb->queens[ci];
                                u32 cj = conflict_queens[j];
                                u32 qj = cb->queens[cj];

                                if (is_attacked(ci, qi) || is_attacked(cj, qj)) {
                                        size_t previous_conflicts = conflict_diagu.nb_set + conflict_diagd.nb_set;

                                        swap(ci, cj, qi, qj);
                                        ++n;

                                        if (previous_conflicts < conflict_diagu.nb_set + conflict_diagd.nb_set) {

                                                swap(ci, cj, qj, qi);
                                                --n;
                                        }

                                }
                        }
                }
        } while (n !=0 || conflict_diagu.nb_set + conflict_diagd.nb_set);


        gettimeofday(&end, NULL);

        log_info("Initialisation completed in %zu.%06ds (%zu conflicts)",
                        before_solve.tv_sec - before_init.tv_sec - (before_solve.tv_usec - before_init.tv_usec < 0 ? 1 : 0),
                        (before_solve.tv_usec - before_init.tv_usec < 0 ? 1000000 : 0) + before_solve.tv_usec - before_init.tv_usec,
                        init_conflicts);
        log_info("Solved in %zu.%06ds",
                        end.tv_sec - before_solve.tv_sec - (end.tv_usec - before_solve.tv_usec < 0 ? 1 : 0),
                        (end.tv_usec - before_solve.tv_usec < 0 ? 1000000 : 0) + end.tv_usec - before_solve.tv_usec);

        log_info("Total time %zu.%06ds",
                        end.tv_sec - before_init.tv_sec - (end.tv_usec - before_init.tv_usec < 0 ? 1 : 0),
                        (end.tv_usec - before_init.tv_usec < 0 ? 1000000 : 0) + end.tv_usec - before_init.tv_usec);

        free(diagu);
        free(diagd);
        free(free_cols);

        bf_dyn_clean(&conflict_diagu);
        bf_dyn_clean(&conflict_diagd);

        return 0;
}
