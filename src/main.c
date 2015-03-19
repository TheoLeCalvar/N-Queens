#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "chessboard.h"

#include "local_search.h"
#include "local_search2.h"
#include "wikimethod.h"
#include "backtrack.h"
#include "forward_checking.h"

typedef int (*algo_f)(cb_t* res);

typedef struct algo {
        char    name[32];
        char    desc[256];
        algo_f  func;
} algo_t;

algo_t algos[] = {
        {"ls",  "local search",         &local_search2},
        {"bt",  "backtrack",            &backtrack},
        {"fw",  "forward checking",     &forward},
        {"bf",  "bruteforce",           NULL},
        {"wk",  "smart wiki",           &wiki_method}
};

#define ALGO_SIZE       (sizeof(algos) / sizeof(algo_t))

int algo_index(const char* name) {
        for (int i = 0; i < ALGO_SIZE; i++) {
                if (!strcmp(name, algos[i].name)) {
                        return i;
                }
        }

        return -1;
}

void algo_help(void) {
        for (int i = 0; i < ALGO_SIZE; i++) {
                printf("    - %-16s : %s\n", algos[i].name, algos[i].desc);
        }
}



void    usage() {
        printf("usage: n-dames [options] size\n"
               "options:\n"
               " --help\tdisplay this message\n"
               " --algo=<algo>\tuse specified the algorithm\n"
               " --image=path\texport chessboard configuration to bmp image\n"
               " algorithms are:\n");
        algo_help();
}

struct option options[] = {
        //{name, has_arg, flag, val}
        {"help",        0, NULL, 'h'},
        {"algo",        1, NULL, 'a'},
        {"image",       0, NULL, 'i'},
        {0,             0, NULL, 0}
};

int main(int argc, char** argv) {
        int     opt;
        int     algo = 0;
        char*   path = NULL;
        size_t  size = 0;
        cb_t    res;
        u32     queens[MAX_QUEENS];

        while ((opt = getopt_long(argc, argv, "a:i:h", options, NULL)) >= 0) {

                switch (opt) {
                        case 'h':
                                usage();
                                return 0;

                        case 'a':
                                algo = algo_index(optarg);
                                if (algo < 0) {
                                        printf("wrong algorithm. Choose one :\n");
                                        algo_help();
                                        return -1;
                                }
                                break;

                        case 'i':
                                path = optarg;
                                break;

                        case '?':
                                usage();
                                return 1;

                        case ':':
                                printf("missing parameter\n\n");
                                usage();
                                return 1;
                }
        }

        if (argc < 2) {
                printf("wrong number of parameters\n");
                usage();
                return 1;
        }

        if (algos[algo].func == NULL) {
                printf("algorithm not yet implement.\n");
                return 1;
        }

        size = atoi(argv[argc - 1]);

        if (size == 1) {
                printf("just put the queen in the only available slot.\n");
                return 1;
        }
        else if (size <= 3) {
                printf("there is no solutions, size needs to be >= 3.\n");
                return 1;
        }

        if (size <= MAX_QUEENS) {
                cb_init(&res, size, &queens);
        }
        else {
                u32 * bf = malloc(size * sizeof(u32));

                if (!bf) {
                        log_err("Can't allocate buffer.");
                        exit(1);
                }

                cb_init(&res, size, bf);
        }

        if (!res.size) {
                return 1;
        }

        if (algos[algo].func(&res)) {
                printf("No solution or an error occured, i'm a sad panda\n");
                return 1;
        }

        cb_print(&res);
        // cb_display(&res);

        if (cb_validates_full(&res)) {
                log_err("The solution isn't valid, this should not happen");
        }


        if (path) {
                cb_to_img(&res, path);
        }

        return 0;
}
