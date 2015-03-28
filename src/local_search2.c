#include "local_search2.h"
#include <time.h>

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
		//if (diag_pos[i + j] && diag_neg[i -j + size] > 0) {
			//if (diag_pos[i + j] > 0)
			diag_pos[i + j]--;
			//else 
			//	diag_pos[i + j] = 0;
			
			//if (diag_neg[i - j + size] > 0)
				diag_neg[i - j + size]--;
			//else
			//	diag_neg[i - j + size] = 0;

			if (diag_pos[i + j] <= 1)
				conflict_pos[i + j]= 0;
			else 
				conflict_pos[i + j]--;
			if (diag_neg[i - j + size] <= 1)
				conflict_neg[i - j + size] = 0;
			else 
				conflict_neg[i - j + size]--;
		//}
		
	}

	void swap_d(size_t size, size_t i,size_t qi,size_t j,size_t qj,size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
		//if (diag_pos[i + qi] > 0 && diag_neg[i - qi + size] > 0 && diag_pos[j + qj] > 0 && diag_neg[j - qj + size] > 0 )
		//{
		sup_queen(size,i,qi,diag_pos,diag_neg,conflict_pos,conflict_neg);
		sup_queen(size,j,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
		add_queen(size,i,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
		add_queen(size,j,qi,diag_pos,diag_neg,conflict_pos,conflict_neg);
		//}
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
		//srand(time(NULL));
		size_t tableau_random[size];
		for (size_t i = 0 ; i < size ; i++) {
			tableau_random[i]=i;
		}	
		size_t indice = 0;
		while (indice < size) {
			size_t rando = rand()%(tail);
			size_t alea = tableau_random[rando];
			//bool ok = true;
			//if (tail > 1000)
			//{
			//	for (size_t j = 0 ; j < size/2 ; ++j) {
			//		if (abs(j - indice) == abs(cb->queens[j] - alea))
			//			ok = false;
			//	} 
			//}
			//if (ok)
			//{
				cb->queens[indice] = alea;
				//size_t tmp = alea;
				tableau_random[rando] = tableau_random[tail-1];
				tail--;
				++indice;
			//}

		}






	}
		




int local_search2(cb_t * cb) {
	size_t size = cb->size;
	//List * list = NULL;
	printf("l'initialisation a commencé \n");
	//cb_init_rand(cb,cb->size);
	init_rand2(cb);
	printf("l'initialisation est terminé \n");
	
	srand(time(NULL));

	size_t * diag_pos = malloc((size * 2) * sizeof(size_t));
	size_t * diag_neg = malloc((size * 2) * sizeof(size_t));
	size_t * conflict_pos = malloc((size * 2) * sizeof(size_t));
	size_t * conflict_neg = malloc((size * 2) * sizeof(size_t));
	//List * tablist[cb->size];
	//size_t diag_pos[(size * 2)-1];
	//size_t diag_neg[(size * 2)-1];


	//size_t conflict_pos[(size * 2)-1];
	//size_t conflict_neg[(size * 2)-1];





	for (size_t i=0; i < (cb->size*2) -1  ; i++) {
			diag_pos[i] = 0;
			//diag_pos[i + cb->size] = 0;
			conflict_pos[i] = 0;
			//conflict_pos[i + cb->size] = 0;
			diag_neg[i] = 0;
			//diag_neg[i + cb->size] = 0;
			conflict_neg[i] = 0;
			//conflict_neg[i + cb->size] = 0;

	}

	init_diagonal(cb, diag_pos, diag_neg, conflict_pos, conflict_neg); 

	size_t c2 = nb_conflict(size*2 -1 ,conflict_neg,conflict_pos);

	unsigned long int swap =0;
	while (c2 > 0) {
			size_t dame_actuel = /*dame_max_conflit(cb->size,tablist)*/rand()%(cb->size);
			size_t qi = cb->queens[dame_actuel];
			if (queen_under_atak(size,dame_actuel,qi,diag_pos,diag_neg )) {
			//int cf_da = list_length(tablist[dame_actuel]);
			//if (cf_da > 0) {
				size_t r1 = rand()%(cb->size);
				size_t qj = cb->queens[r1];
				if (r1 != dame_actuel ) {
					
					swap++;
					swap_d(size,dame_actuel,qi,r1,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
					cb_swap(cb,dame_actuel,r1);
					//init_diagonal(cb, diag_pos, diag_neg, conflict_pos, conflict_neg);
					int ctmp =  nb_conflict(size*2 -1,conflict_neg,conflict_pos);
					if (ctmp > c2) {
					//if (nb_conflict(size*2,conflict_neg,conflict_pos) > c2) {
						swap_d(size,r1,qi,dame_actuel,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
						cb_swap(cb,r1,dame_actuel);
						//init_diagonal(cb, diag_pos, diag_neg, conflict_pos, conflict_neg);

					}
					else {
						c2 = ctmp;
						//printf("%d\n",c2);

						
					}
						//c2 = nb_conflict(size*2,conflict_neg,conflict_pos);
					//int cconflit = cb_conflicts_point_list(cb,tablist,dame_actuel,r1);
					//if (list_length(tablist[dame_actuel]) < cf_da)
						//dame_actuel++;
					//else 
						//cb_swap(cb,r1,dame_actuel);
				}

			}
		}
		//affiche_diag(size*2-1,diag_pos);
		//affiche_diag(size*2-1,diag_neg);
		printf("il y a eu %lu swap mon captain \n",swap);
		
		//int c1 = cb_conflicts(cb,buf);
		free(diag_pos);
		free(diag_neg);
		free(conflict_pos);
		free(conflict_neg);


		
	
	return 0;
	


}
