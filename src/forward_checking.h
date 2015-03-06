#ifndef _forward_checking_h_
#define _forward_checking_h_

#include "chessboard.h"

#define ROW_MASK        0x01
#define DIAG1_MASK      0x02 
#define DIAG2_MASK      0x04      

int forward(cb_t* cb);

#endif