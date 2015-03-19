#ifndef _local_search_h2_
#define _local_search_h2_

#include "chessboard.h"

#define diagp(i, j, size) \
		(i + j)

#define diagm(i, j, size) \
		(i - j + size)

int     local_search2(cb_t* cb);

//void	init_matrice(const cb_t* cb, size_t** m); 
#endif