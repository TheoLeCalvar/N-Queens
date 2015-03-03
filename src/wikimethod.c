#include "wikimethod.h"

int wiki_method(cb_t * cb)
{
	size_t k = 1;
	size_t n = cb->size ;
	size_t y = 0;

	size_t i = 1;
		for (; i <= n/2 /* + (n%2) ? 1 : 0*/; i++, k += 2) {
			cb->queens[i-1] =  k;
			//printf("%d",i );
		}	
		for (; i <= n; i++, y += 2) {
			//printf("%d, %d\n", i, k);
			cb->queens[i-1] = y;
		}
	//if((n % 6) != 2 && (n % 6) != 3) {}
	

		
	if ((n % 6) == 2) {


		for (size_t i = 0; i < n; i++) {
			if (cb->queens[i] == 0)
				cb->queens[i] = 2;
			else if (cb->queens[i] == 2)
				cb->queens[i] = 0;
			else if (cb->queens[i] == 4) {
				for (size_t j = i; j < n-1; j++) {
					cb->queens[j] = cb->queens[j+1];
				}
				cb->queens[n-1] = 4;
			}
			
			
		}
		for (size_t i = 0; i < n; i++)
		printf("%d ", cb->queens[i]+1);

	}
	if ((n % 6) == 3) {
		for (size_t i = 0; i < n; i++) {
			if (cb->queens[i] == 1) {
				for (size_t j = i ; j < n/2 ; j++) {
					cb->queens[j] = cb->queens[j+1];
				}
				cb->queens[n/2] = 1;
			}
			else if (cb->queens[i] == 2) {
				for (size_t j = i; j < n-1; j++) {
					cb->queens[j] = cb->queens[j+1];
				}
				//cb->queens[n-1] = 2;
			}
			else if (cb->queens[i] == 0) {
				for (size_t j = i; j < n-1; j++) {
					cb->queens[j] = cb->queens[j+1];
				}
				//cb->queens[n-1] = 0;
			}
		}
		cb->queens[n-2] = 0;
		cb->queens[n-1] = 2;
		for (size_t i = 0; i < n; i++)
		printf("%d ", cb->queens[i]+1);
	}	



	cb_print(cb);
	return cb_validates(cb);
}