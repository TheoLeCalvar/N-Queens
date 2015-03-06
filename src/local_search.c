#include "local_search.h"

/*extern void init_cb_rand(cb_t* cb) {
        for (size_t i = 0; i < cb->size; ++i) {
                cb->queens[i] = cb_best_row(cb, i);
        }

        cb_print(cb);
}*/

/*void    init_matrice(const cb_t *cb,size_t **mc) {
        for (size_t i = 0; i < cb->size ; ++i) {
                for (size_t j = 0; j < cb->size ; ++j) {
                        mc[i][j]=0;
                
                }
        }
        for (size_t i = 0; i < cb->size ; ++i) {
                if (cb->queens[i] != -1) {
                        size_t k = cb->queens[i];
                        //mc[i][k]++;


                }

        }        

}*/

int     local_search(cb_t* cb) {
        //size_t matrice_conflit[cb->size][cb->size];
        cb_init_rand(cb,cb->size);
        srand(time(NULL));
        u32 *   buf;
        double T = 100;
        //size_t tmp = 0;
        //size_t tmp2 = 0;
        buf = malloc(cb->size * sizeof(u32));
        while (cb_conflicts(cb,buf) > 0 ) {
                        int c = cb_conflicts(cb,buf);
                        /*for (int i = 0; i < cb->size; i++) {

                                if (tmp < buf[i]) {
                                        tmp = buf[i];
                                        tmp2 = i;
     
                                }
                        }ca put la merde et ralentie le truc */
                        size_t r1 = rand()%(cb->size);
                        size_t r2 = rand()%(cb->size);
                        if (r1 != r2) {
                                cb_swap(cb,r1,r2);

                                if (cb_conflicts(cb,buf) > c) {
                                        double deltaf = (cb_conflicts(cb,buf) - c);
                                        if ((exp(-deltaf/T)) < 0.5)
                                                T = 0.6 * T;
                                        else
                                                cb_swap(cb,r1,r2);
                                }
                                               
                        }
                         
                }
        return cb_validates(cb);       
}
/*      u32 *   buf_col;
        u32 *   buf_row;

        init_cb(cb);
        buf_col = malloc(cb->size * sizeof(u32));
        buf_row = malloc(cb->size * sizeof(u32));

        if (!buf_col || !buf_row) {
                log_err("Can't allocate buffer");
                return 1;
        }

        if (cb->size == -1) {cb
                log_err("Can't allocate chessboard");
                return 1;
        }

        while (cb_conflicts(cb, buf_col)) {
                
        }

        printf("Found a solution in %zu switchs.\n", try);
        */
       


#ifdef TEST

#include "test.h"

int main(int argc, char **argv) {

}

#endif