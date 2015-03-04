#ifndef _chessboard_h_
#define _chessboard_h_

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

#include "types.h"
#include "bf.h"

#define MAX_QUEENS    4096

typedef struct chessboard {
        size_t          size;
        bf_t            cols; //bitset, available cols, 1 empty, 0 full
        bf_t            rows; // bitset, available rows
        u32             queens[MAX_QUEENS]; //position of the queen
} cb_t;

void    cb_init(cb_t* cb, size_t size);

//returns 0 if the chessboard is valid, < 0 if uninitialized, > 0 if invalid
int     cb_validates(const cb_t* cb);

int     cb_validates_fast(const cb_t* cb, size_t row, size_t col);

//fill buff with the number of conflict per queen, buf must be long enought
//if buff is NULL, it only return the number of conflicting queens
//returns the number of conflicting queens, -1 if cb is invalid 
int     cb_conflicts(const cb_t* cb, u32* buf);


size_t  cb_best_row(cb_t* cb, size_t col);

//fill buf with the number of conflict for each row yo can place the queens of the given col
void    cb_rows(cb_t*, size_t col, u32* buf);

void    cb_print(const cb_t* cb);

//mettre en place évaluation incrémentale (reprendre le résultat précédent du calcul)
//forcer une reine par ligne/colonne, passer sur 2 tableaux ?
//un pour les lignes et un pour les colonnes ?
//rajouter un tableau pour les colonnes,modifier best_row pour faire un swap des deux


#endif