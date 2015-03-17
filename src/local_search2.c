#include "local_search2.h"

typedef struct s_List List;
	struct s_List {
		List *next;
		int qconf;
	};

	//List *list = NULL;

	List * list_create(int q) {
		List * list= malloc(sizeof(list));
		if (list)
		{
			list->qconf = q;
			list->next = NULL;
		}
		return list;
	}

	List * list_adhead(List * old,int data) {
		List *list = list_create(data);
		if(list)
			list->next = old;
		return list;
	}

	List * list_append(List *list, int data) {
	    List **plist = &list;
	    while (*plist)
	       plist = &(*plist)->next;
	    *plist = list_create(data);
	    if (*plist)
	       return list;
	    else
	       return NULL;
	}
	List * list_remove_first(List *list) {
		List *first = list;
		list = list->next;
		free(first);
		return list;
	}
	void list_destroy(List *list) {
		while (list)
			list = list_remove_first(list);
	}
	int list_length(List *list) {
		int length = 0;
		while (list) 
		{
			length++;
			list = list->next;
		}
		return length;
	}
	void list_affiche(List * list) {
		while (list) {
			//printf("hello world ");
			printf("%d ",list->qconf);
			list = list->next;

		}
	}
	int cb_conflict_list(const cb_t * cb,List ** tabli) {
		if (!cb->size) {
                log_err("Call cb_conflicts on non valid cb");
                return -1;
        }
        int conflict = 0;
        for (size_t i = 0; i < cb->size; i++)
        	if (cb->queens[i] != -1)
        		for (size_t j = i+1; j < cb->size; ++j)
        			if (cb->queens[j] != -1 &&
                    	(cb->queens[i] == cb->queens[j] ||
                        abs(i - j) == abs(cb->queens[i] - cb->queens[j]))
                    ) {
             
        					tabli[i] = list_adhead(tabli[i],j);
        					tabli[j] = list_adhead(tabli[j],i);
        				
        					++conflict;
        			}

        return conflict;
	}
	int cb_conflicts_point_list(const cb_t * cb, List ** tabli, size_t ligne, size_t col) {
		int conflict = 0;
		u32 queen; 
		for (size_t i = 0; (i < cb->size) && ((queen = cb->queens[i]) != -1); ++i) {
                if (    i != col &&
                        abs(i - col) == abs(queen - ligne)
                   ) {
                   	tabli[i] = list_adhead(tabli[i],col);
                   	tabli[col] = list_adhead(tabli[col],i);
                        ++conflict;
                   }
        }

        return conflict;
	}
	bool list_isinside(List * list, int x) {
		while (list) {
			if (list->qconf == x)
				return true;
			else list = list->next;
		}
		return false; 
	}
	/*size_t dame_max_conflit(size_t taille, List **tablist) {
		size_t max = 0;
		for (size_t i = 0; i < taille ; i++) {
			if(list_length(tablist[i]) > max)
				max = i;
		}
	}*/
	void init_diagonal(const cb_t * cb,size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
		for(size_t i=0; i < cb->size; i++) {
			
			diag_pos[diagp(i,cb->queens[i],cb->size)] ++;
			if (diag_pos[diagp(i,cb->queens[i],cb->size)] > 1)
				conflict_pos[diagp(i,cb->queens[i],cb->size)]++;
			
			diag_neg[diagm(i,cb->queens[i],cb->size)] ++;
			if (diag_neg[diagm(i,cb->queens[i],cb->size)] > 1)
				conflict_neg[diagm(i,cb->queens[i],cb->size)]++;
		}
	}
	void add_queen(size_t size,size_t i, size_t j,size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
		diag_pos[diagp(i,j,size)]++;
		diag_neg[diagm(i,j,size)]++;
		if (diag_pos[diagp(i,j,size)] > 1)
			conflict_pos[diagp(i,j,size)] ++;
		if (diag_neg[diagm(i,j,size)] > 1)
			conflict_neg[diagm(i,j,size)] ++;

	}
	void sup_queen(size_t size, size_t i,  size_t j, size_t * diag_pos,size_t * diag_neg,size_t * conflict_pos, size_t * conflict_neg) {
		diag_pos[i + j]--;
		diag_neg[i - j + size]--;
		if (diag_pos[i + j] <= 1)
			conflict_pos[i + j]= 0;
		else 
			diag_pos[i + j]--;
		if (diag_neg[i - j + size] <= 1)
			conflict_neg[i - j + size] = 0;
		else 
			diag_neg[i - j + size]--;
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

	/*void init_conflit(size_t size,const size_t * dp, const size_t * dn, size_t * cp, size_t * cn) {
		size_t c1 = 0;
		size_t c2 = 0;
		for (size_t i = 0 ; i < (size * 2) ; i++) {
			c1 += diag_pos[i];
			c2 += diag_neg[i];	

		}


	}*/		



int local_search2(cb_t * cb) {
	//List * list = NULL;
	printf("l'initialisation a commencé \n");
	cb_init_rand(cb,cb->size);
	printf("l'initialisation est terminé \n");
	size_t size = cb->size;
	srand(time(NULL));
	List * tablist[cb->size];
	size_t diag_pos[(cb->size * 2)];
	size_t diag_neg[(cb->size * 2)];


	size_t conflict_pos[cb->size * 2];
	size_t conflict_neg[cb->size * 2];





	for (size_t i=0; i < cb->size  ; i++) {
			diag_pos[i] = 0;
			diag_pos[i + cb->size] = 0;
			conflict_pos[i] = 0;
			conflict_pos[i + cb->size] = 0;
			diag_neg[i] = 0;
			diag_neg[i + cb->size] = 0;
			conflict_neg[i] = 0;
			conflict_neg[i + cb->size] = 0;

	}

	init_diagonal(cb, diag_pos, diag_neg, conflict_pos, conflict_neg); 
	//affiche_diag(size,diag_pos);
	for (size_t i = 0; i < cb->size; i++) {
		tablist[i] = NULL;
	}
	u32 * buf ;
	buf = malloc(cb->size *sizeof(u32));
	int c = 0;
	int c3 = cb_conflicts(cb,buf);
	c = cb_conflict_list(cb,tablist);
	size_t c2 = nb_conflict(size*2,conflict_neg,conflict_pos);
	if (c != c2)
		printf("eu probleme mon capitain ? %d , %d  , %d \n",c,c2,c3);
	/*for (size_t i = 0; i < cb->size; ++i) {
		printf("colone numero %d \n",i);
		list_affiche(tablist[i]);
	}*/
	//printf("ok ? \n");
	//size_t dame_actuel = 0;
	while (c2 > 0) {
		
			size_t dame_actuel = /*dame_max_conflit(cb->size,tablist)*/rand()%(cb->size);
			size_t qi = cb->queens[dame_actuel];
			if (queen_under_atak(size,dame_actuel,qi,diag_pos,diag_neg )) {
			//int cf_da = list_length(tablist[dame_actuel]);
			//if (cf_da > 0) {
				size_t r1 = rand()%(cb->size);
				size_t qj = cb->queens[r1];
				if (r1 != dame_actuel ) {
					
					
					swap_d(size,dame_actuel,qi,r1,qj,diag_pos,diag_neg,conflict_pos,conflict_neg);
					cb_swap(cb,dame_actuel,r1);
					//if (cb_conflicts(cb,buf) > c) {
					if (nb_conflict(size*2,conflict_neg,conflict_pos) > c2) {
						swap_d(size,r1,qj,dame_actuel,qi,diag_pos,diag_neg,conflict_pos,conflict_neg);
						cb_swap(cb,r1,dame_actuel);
					}
					else
						c2 = nb_conflict(size*2,conflict_neg,conflict_pos);
						//c = cb_conflict_list(cb,tablist);
					//int cconflit = cb_conflicts_point_list(cb,tablist,dame_actuel,r1);
					//if (list_length(tablist[dame_actuel]) < cf_da)
						//dame_actuel++;
					//else 
						//cb_swap(cb,r1,dame_actuel);
				}

			}
		}
		
		//int c1 = cb_conflicts(cb,buf);

		
	
	return 0;
	


}