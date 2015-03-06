#include "forward_checking.h"

//a terme, adapter chessboard, proposer un bt_t[MAXQ_QUEENS], et pour chaque reine placée 
//mettre à jour les autres en modifiant les domaines

//problème, il faudra trouver un moyen de garder les différences entre les domaines


//amélioration possible, pour la ligne, réduire les valeurs possible en supprimant les
//valeur dans les diagonales des reines précédentes
extern int fw_rec(cb_t* cb, bf_t* domains, size_t col) {
        //weare not at the end, now we have to select where to place the colth queens
        if (col < cb->size) {
                size_t  queen_place = -1;
                size_t  min_dom = -1;
                size_t  delta_size = 0;;
                size_t  delta[MAX_QUEENS * 4];
                //      delta[queens] = {i, j, k, l}
                //      row, diag +, diag -, count

                //now we get our first available queen in the remaining domain
                while ((queen_place = bf_get_next_setted(&domains[col], queen_place)) < cb->size) {
                        //we place the queen in the first available slot
                        cb->queens[col] = queen_place;

                        // cb_print(cb);

                        //we update the domains of unplaced queens
                        for (delta_size = 0; delta_size < cb->size; ++delta_size) {

                                delta[4 * delta_size    ] = -1;
                                delta[4 * delta_size + 1] = -1;
                                delta[4 * delta_size + 2] = -1;
                                delta[4 * delta_size + 3] = -1;

                                if (cb->queens[delta_size] != -1)
                                        continue;

                                if (bf_get(domains[delta_size].field, queen_place)) {
                                        delta[4 * delta_size    ] = 1;
                                        bf_unset(domains[delta_size].field, queen_place);
                                }

                                if (queen_place + (col - delta_size) < MAX_QUEENS &&
                                    bf_get(domains[delta_size].field, queen_place + (col - delta_size))
                                    ) {
                                        delta[4 * delta_size + 1] = 1;
                                        bf_unset(domains[delta_size].field, queen_place +  (col - delta_size));
                                }

                                if (queen_place - (col - delta_size) < MAX_QUEENS &&
                                    bf_get(domains[delta_size].field, queen_place - (col - delta_size))
                                    ) {
                                        delta[4 * delta_size + 2] = 1;
                                        bf_unset(domains[delta_size].field, queen_place - (col - delta_size));
                                }

                                if (bf_get_next_setted(&domains[delta_size], -1) > cb->size)
                                        goto restore;

                                delta[4 * delta_size + 3] = bf_count(&domains[delta_size]);

                                //y'a un bug dans delta[min_dom], mais c'est plus rapide et toujours valide ...
                                if (min_dom == -1 || delta[4 * delta_size + 3] < delta[min_dom])
                                        min_dom = delta_size;
                        }     


                        // cb_display(cb);
                        // cb_print(cb);
                        // bf_print(&domains[col], cb->size);
                        // printf("%d\n", queen_place);

                        //recursive call with updated domains
                        if (!fw_rec(cb, domains, min_dom))
                                return 0;

restore:
                        //else we have to undo what we have done and try the next place
                        cb->queens[col] = -1;
                        min_dom = -1;


                        for (size_t i = 0; i <= delta_size; ++i) {
                                if (delta[4 * i    ] == 1)
                                        bf_set(domains[i].field, queen_place);

                                if (queen_place + (col - i) < MAX_QUEENS &&
                                    delta[4 * i + 1] == 1) 
                                        bf_set(domains[i].field, queen_place + (col - i));

                                if (queen_place - (col - i) < MAX_QUEENS &&
                                    delta[4 * i + 2] == 1)
                                        bf_set(domains[i].field, queen_place - (col - i));    
                        }

                }

                return 1;
                
        }
        //we are at the bottom, only one choice, evaluate the leaf
        else
                return cb_validates(cb);
}


int forward(cb_t* cb) {
        bf_t    domains[MAX_QUEENS];

        for (size_t i = 0; i < cb->size; ++i) 
                bf_init(&domains[i], 1);
        

        return fw_rec(cb, domains, 0);
}