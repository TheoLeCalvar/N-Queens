#include "local_search2.h"
#include <time.h>

#define diagp(i, j, size) \
		(i + j)

#define diagm(i, j, size) \
		(i - j + size)

void init_diagonal(const cb_t * cb,size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
	for(size_t i=0; i < cb->size ; i++) {

		diag_pos[diagp(i,cb->queens[i],cb->size)] ++;
		if (diag_pos[diagp(i,cb->queens[i],cb->size)] > 1)
			conflict_pos[diagp(i,cb->queens[i],cb->size)]++;

		diag_neg[diagm(i,cb->queens[i],cb->size)] ++;
		if (diag_neg[diagm(i,cb->queens[i],cb->size)] > 1)
			conflict_neg[diagm(i,cb->queens[i],cb->size)]++;
	}
}
void add_queen(size_t size,size_t i, size_t j,size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
	diag_pos[i +j]++;
	diag_neg[i - j + size]++;
	if (diag_pos[i+j] > 1)
		conflict_pos[i+j] ++;
	if (diag_neg[i - j + size] > 1)
		conflict_neg[i - j + size] ++;

}
void sup_queen(size_t size, size_t i,  size_t j, size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
	diag_pos[i + j]--;

	diag_neg[i - j + size]--;

	if (diag_pos[i + j] <= 1)
		conflict_pos[i + j]= 0;
	else
		conflict_pos[i + j]--;

	if (diag_neg[i - j + size] <= 1)
		conflict_neg[i - j + size] = 0;
	else
		conflict_neg[i - j + size]--;
}

void swap_d(size_t size, size_t i,size_t qi,size_t j,size_t qj,size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
	sup_queen(size,i,qi,diag_pos,diag_neg,conflict_pos,conflict_neg);
	sup_queen(size,j,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
	add_queen(size,i,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
	add_queen(size,j,qi,diag_pos,diag_neg,conflict_pos,conflict_neg);
}
bool queen_under_atak(size_t size, size_t i,size_t j, size_t * diag_pos, size_t * diag_neg) {
	return (diag_pos[i + j] > 1 || diag_neg[i - j + size] > 1);
}
void affiche_diag(size_t size,size_t * diag_pos) {
	printf("affichage ?\n");
	for (size_t i=0 ; i < size; i++) {
		printf("%d, ",diag_pos[i]);
	}
}
size_t sum_tab(size_t size, size_t * conflict_pos) {
	size_t value = 0;
	for (size_t i = 0; i < size; ++i)
	{
		value += conflict_pos[i];
	}
	return value;
}
size_t nb_conflict(size_t size, size_t * conflict_neg, size_t * conflict_pos) {
	return (sum_tab(size,conflict_neg)+sum_tab(size,conflict_pos));
}
void init_rand2(cb_t * cb) {

	size_t size = cb->size;
	for (size_t i = 0; i < size; ++i) {
		cb->queens[i] = -1;
	}

	size_t tail = size;
	size_t tableau_random[size];
	for (size_t i = 0 ; i < size ; i++) {
		tableau_random[i]=i;
	}
	size_t indice = 0;
	while (indice < size) {
		size_t rando = rand()%(tail);
		size_t alea = tableau_random[rando];

		cb->queens[indice] = alea;
		tableau_random[rando] = tableau_random[tail-1];
		tail--;
		++indice;

	}
}





int local_search2(cb_t * cb) {
	size_t size = cb->size;

	printf("l'initialisation a commencé \n");

	init_rand2(cb);
	printf("l'initialisation est terminé \n");

	size_t * diag_pos = malloc((size * 2) * sizeof(size_t));
	size_t * diag_neg = malloc((size * 2) * sizeof(size_t));
	size_t * conflict_pos = malloc((size * 2) * sizeof(size_t));
	size_t * conflict_neg = malloc((size * 2) * sizeof(size_t));



	for (size_t i=0; i < (cb->size*2) -1  ; i++) {
			diag_pos[i] = 0;
			conflict_pos[i] = 0;
			diag_neg[i] = 0;
			conflict_neg[i] = 0;

	}

	init_diagonal(cb, diag_pos, diag_neg, conflict_pos, conflict_neg);

	size_t c2 = nb_conflict(size*2 -1 ,conflict_neg,conflict_pos);

	unsigned long int swap =0;
	while (c2 > 0) {
		size_t dame_actuel = rand()%(cb->size);
		size_t qi = cb->queens[dame_actuel];

		if (queen_under_atak(size,dame_actuel,qi,diag_pos,diag_neg )) {

			size_t r1 = rand()%(cb->size);
			size_t qj = cb->queens[r1];
			if (r1 != dame_actuel ) {

				swap++;
				swap_d(size,dame_actuel,qi,r1,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
				cb_swap(cb,dame_actuel,r1);
				int ctmp =  nb_conflict(size*2 -1,conflict_neg,conflict_pos);
				if (ctmp > c2) {

					swap_d(size,r1,qi,dame_actuel,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
					cb_swap(cb,r1,dame_actuel);

				}
				else {
					c2 = ctmp;
				}
			}

		}
	}

	printf("il y a eu %lu swap mon captain \n",swap);

	free(diag_pos);
	free(diag_neg);
	free(conflict_pos);
	free(conflict_neg);


	return 0;



}
