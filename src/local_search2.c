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
	size_t dame_max_conflit(size_t taille, List **tablist) {
		size_t max = 0;
		for (size_t i = 0; i < taille ; i++) {
			if(list_length(tablist[i]) > max)
				max = i;
		}
	}


int local_search2(cb_t * cb) {
	//List * list = NULL;
	printf("l'initialisation a commencé \n");
	cb_init_rand(cb,cb->size);
	printf("l'initialisation est terminé \n");

	srand(time(NULL));
	List * tablist[cb->size];
	for (size_t i = 0; i < cb->size; i++) {
		tablist[i] = NULL;
	}
	u32 * buf ;
	buf = malloc(cb->size *sizeof(u32));
	int c = 0;
	c = cb_conflict_list(cb,tablist);
	if(cb_validates(cb))
	{
		printf("l'initialisation a fonctionné \n");
	}

	/*for (size_t i = 0; i < cb->size; ++i) {
		printf("colone numero %d \n",i);
		list_affiche(tablist[i]);
	}*/
	//printf("ok ? \n");
	//size_t dame_actuel = 0;
	while (c > 0) {
		
			size_t dame_actuel = /*dame_max_conflit(cb->size,tablist)*/rand()%(cb->size);
			int cf_da = list_length(tablist[dame_actuel]);
			if (cf_da > 0) {
				size_t r1 = rand()%(cb->size);
				if (r1 != dame_actuel && (list_isinside(tablist[dame_actuel],r1)== false)) {
					cb_swap(cb,dame_actuel,r1);
					if (cb_conflicts(cb,buf) > c)
						cb_swap(cb,r1,dame_actuel);
					else
						c = cb_conflict_list(cb,tablist);
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