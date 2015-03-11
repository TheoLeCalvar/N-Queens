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

int local_search2(cb_t * cb) {
	//List * list = NULL;
	cb_init_rand(cb,cb->size);

	List * tablist[cb->size];
	for (size_t i = 0; i < cb->size; i++) {
		tablist[i] = NULL;
	}
	//u32 * buf ;
	//buf = malloc(cb->size *sizeof(u32));
	int c = 0;
	c = cb_conflict_list(cb,tablist);

	for (size_t i = 0; i < cb->size; ++i) {
		printf("colone numero %d \n",i);
		list_affiche(tablist[i]);
	}
	printf("ok ? \n");
	return 0;
	


}