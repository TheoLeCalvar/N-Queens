#ifndef _chessboard_h_
#define _chessboard_h_
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#include "log.h"
//#include <math.h>

#include "types.h"
#include "bf.h"

#define MAX_QUEENS    4096

typedef struct chessboard {
        size_t          size;
        u32*            queens; //position of the queen
} cb_t;

void    cb_init(cb_t* cb, size_t size, u32* buff);

void	cb_init_rand(cb_t* cb,size_t size);
//returns 0 if the chessboard is valid, < 0 if uninitialized, > 0 if invalid
int     cb_validates(const cb_t* cb);

int     cb_validates_full(const cb_t* cb);

void	cb_swap(cb_t* cb, size_t q1, size_t q2);

void    cb_print(const cb_t* cb);

void    cb_display(const cb_t* cb);

void    cb_to_img(const cb_t* cb, const char* path);

void    cb_to_01(const cb_t* cb);


//mettre en place évaluation incrémentale (reprendre le résultat précédent du calcul)
//forcer une reine par ligne/colonne, passer sur 2 tableaux ?
//un pour les lignes et un pour les colonnes ?
//rajouter un tableau pour les colonnes,modifier best_row pour faire un swap des deux


#endif
