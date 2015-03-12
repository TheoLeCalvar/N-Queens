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
                size_t  min_dom_val = -1;
                size_t  tmp_count;
                size_t  delta_size = 0;
                u8      delta[MAX_QUEENS];
                //ROW_MASK, DIAG1_MASK and DIAG2_MASK, save  if we need to revert the bit or not

                //just to be safe, but this should not happen
                if (cb->queens[col] != -1)
                        return 1;

                //now we get our first available queen in the remaining domain
                while ((queen_place = bf_get_next_setted(&domains[col], queen_place)) < cb->size) {
                        //we place the queen in the first available row
                        cb->queens[col] = queen_place;

                        // printf("We are in (%zu, %zu) - ", col, queen_place);
                        // bf_print(&domains[col], cb->size);

                        //we update the domains of unplaced queens
                        for (delta_size = 0; delta_size < cb->size; ++delta_size) {

                                delta[delta_size] = 0;

                                if (cb->queens[delta_size] != -1)
                                        continue;

                                //UPDATE, utiliser 0x01, 0x02, 0x04 et 0x08 !!!
                                if (bf_get(domains[delta_size].field, queen_place)) {
                                        delta[delta_size] |= ROW_MASK;
                                        bf_unset(domains[delta_size].field, queen_place);
                                }

                                if (queen_place + (col - delta_size) < cb->size &&
                                    bf_get(domains[delta_size].field, queen_place + (col - delta_size))
                                    ) {
                                        delta[delta_size] |= DIAG1_MASK;
                                        bf_unset(domains[delta_size].field, queen_place + (col - delta_size));
                                        // printf("We are in (%zu,%zu) and we unset (%zu, %zu)\n", col, queen_place, delta_size, queen_place + (col - delta_size));
                                }
                                //POIURQUOI ÇA FAIT CRASH LE PROGRAMME SUR CERTAINES VALEURS !?8?88?
                                if (queen_place - (col - delta_size) < cb->size &&
                                    bf_get(domains[delta_size].field, queen_place - (col - delta_size))
                                    ) {
                                        delta[delta_size] |= DIAG2_MASK;
                                        bf_unset(domains[delta_size].field, queen_place - (col - delta_size));
                                        // printf("We are in (%zu,%zu) and we unset (%zu, %zu)\n", col, queen_place, delta_size, queen_place - (col - delta_size));
                                }

                                // bf_print(&domains[delta_size], cb->size);

                                tmp_count = bf_count(&domains[delta_size]);

                                //the domain is empty, no need to go further, clean and test the next place
                                if (tmp_count == 0)
                                        goto restore;

                                //if the domain is smaller we save it for the next step
                                if (min_dom == -1 || tmp_count < min_dom_val) {
                                        min_dom = delta_size;
                                        min_dom_val = tmp_count;
                                }
                        }     


                        //we check the smallest domain first, cause it's better !
                        if (!fw_rec(cb, domains, min_dom))
                                return 0;

                        // printf("Back from recursion (%zu, %zu)\n", col, queen_place);
                       
restore:
                        //else we have to undo what we have done and try the next place
                        cb->queens[col] = -1;
                        min_dom = -1;


                        for (size_t i = 0; i < cb->size && i <= delta_size; ++i) {
                                if (delta[i] & ROW_MASK)
                                        bf_set(domains[i].field, queen_place);

                                //no need to check if value is in bound, it has already been done when set
                                if (delta[i] & DIAG1_MASK) 
                                        bf_set(domains[i].field, queen_place + (col - i));

                                //idem
                                if (delta[i] & DIAG2_MASK)
                                        bf_set(domains[i].field, queen_place - (col - i));    
                        }

                }
                // printf("Backtrack (%zu)\n", col);
                return 1;
                
        }
        //we are at the bottom, only one choice, evaluate the leaf
        else
                return cb_validates_full(cb);
}


int forward(cb_t* cb) {
        bf_t    domains[MAX_QUEENS];

        //we set the cb->size first bits to 1
        for (size_t i = 0; i < cb->size; ++i) {
                bf_init_from(&domains[i], cb->size);
                bf_not(&domains[i]);
        }
        

        return fw_rec(cb, domains, 0);
}